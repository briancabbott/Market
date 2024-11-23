/*
 * SPDX-FileCopyrightText: 2024 Binsparse Developers
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <assert.h>
#include <binsparse/array.h>
#include <hdf5.h>
#include <string.h>

#include <unistd.h>

#include <binsparse/detail/shm_tools.h>

#if __STDC_VERSION__ >= 201112L
#include <stdatomic.h>
#endif

// Write an array to a dataset / file
// Returns 0 on success, nonzero on error.
static inline int bsp_write_array(hid_t f, const char* label, bsp_array_t array,
                                  int compression_level) {
  if (array.type == BSP_COMPLEX_FLOAT32 || array.type == BSP_COMPLEX_FLOAT64) {
    array = bsp_complex_array_to_fp(array);
  }

  hsize_t hsize[1];

  hsize[0] = array.size;

  hid_t hdf5_standard_type = bsp_get_hdf5_standard_type(array.type);
  hid_t fspace = H5Screate_simple(1, hsize, NULL);
  hid_t lcpl = H5Pcreate(H5P_LINK_CREATE);

  hid_t dcpl = H5Pcreate(H5P_DATASET_CREATE);

  // Choose 1 MiB, the default chunk cache size, as our chunk size.
  size_t chunk_size = 1024 * 1024 / bsp_type_size(array.type);

  // If the dataset is smaller than the chunk size, cap the chunk size.
  if (array.size < chunk_size) {
    chunk_size = array.size;
  }

  hsize[0] = chunk_size;
  H5Pset_chunk(dcpl, 1, hsize);

  if (compression_level > 0) {
    H5Pset_deflate(dcpl, compression_level);
  }

  hid_t dset =
      H5Dcreate2(f, label, hdf5_standard_type, fspace, lcpl, dcpl, H5P_DEFAULT);

  if (dset == H5I_INVALID_HID) {
    return -1;
  }

  hid_t hdf5_native_type = bsp_get_hdf5_native_type(array.type);

  hid_t r = H5Dwrite(dset, hdf5_native_type, H5S_ALL, fspace, H5P_DEFAULT,
                     array.data);

  if (r == H5I_INVALID_HID) {
    return -2;
  }

  H5Sclose(fspace);
  H5Pclose(lcpl);
  H5Pclose(dcpl);

  return 0;
}

#if __STDC_VERSION__ >= 201112L
static inline bsp_array_t bsp_read_array_parallel(hid_t f, const char* label,
                                                  int num_threads) {
  hid_t dset = H5Dopen2(f, label, H5P_DEFAULT);

  if (dset == H5I_INVALID_HID) {
    return bsp_construct_default_array_t();
  }

  hid_t fspace = H5Dget_space(dset);

  if (fspace == H5I_INVALID_HID) {
    return bsp_construct_default_array_t();
  }

  hsize_t dims[3];

  int r = H5Sget_simple_extent_dims(fspace, dims, NULL);

  if (r < 0) {
    return bsp_construct_default_array_t();
  }

  hid_t hdf5_type = H5Dget_type(dset);

  bsp_type_t type = bsp_get_bsp_type(hdf5_type);

  // Array will be written into a POSIX shared memory.
  bsp_shm_t array_shm = bsp_shm_new(dims[0] * bsp_type_size(type));
  bsp_array_t array;
  array.type = type;
  array.size = dims[0];
  array.allocator = bsp_shm_allocator;

  bsp_shm_t active_children_shm = bsp_shm_new(sizeof(_Atomic int));

  _Atomic int* active_children = bsp_shm_attach(active_children_shm);
  bsp_shm_delete(active_children_shm);

  *active_children = num_threads - 1;

  pid_t* pids = (pid_t*) malloc(sizeof(pid_t) * num_threads);

  int thread_num = 0;

  for (size_t i = 0; i < num_threads - 1; i++) {
    pid_t new_pid = fork();

    if (new_pid != 0) {
      pids[i] = new_pid;
    } else {
      thread_num = i + 1;
      break;
    }
  }

  array.data = bsp_shm_attach(array_shm);
  if (thread_num == 0) {
    bsp_shm_delete(array_shm);
  }

  hsize_t chunk_size = (array.size + num_threads - 1) / num_threads;
  hsize_t start = (chunk_size * thread_num < array.size)
                      ? chunk_size * thread_num
                      : array.size;
  hsize_t count =
      (start + chunk_size <= array.size) ? chunk_size : array.size - start;

  if (count > 0) {
    H5Sselect_hyperslab(fspace, H5S_SELECT_SET, &start, NULL, &count, NULL);

    hid_t memspace_id = H5Screate_simple(1, &count, NULL);

    H5Dread(dset, bsp_get_hdf5_native_type(type), memspace_id, fspace,
            H5P_DEFAULT, ((char*) array.data) + start * bsp_type_size(type));
    H5Sclose(memspace_id);
  }

  H5Dclose(dset);
  H5Sclose(fspace);

  if (thread_num > 0) {
    atomic_fetch_add_explicit(active_children, -1, memory_order_relaxed);
    bsp_shm_detach(active_children);
    bsp_shm_detach(array.data);
    exit(0);
  }

  free(pids);

  while (atomic_load(active_children) > 0) {
  }
  bsp_shm_detach(active_children);

  return array;
}
#endif

static inline bsp_array_t bsp_read_array(hid_t f, const char* label) {
  hid_t dset = H5Dopen2(f, label, H5P_DEFAULT);

  if (dset == H5I_INVALID_HID) {
    return bsp_construct_default_array_t();
  }

  hid_t fspace = H5Dget_space(dset);

  if (fspace == H5I_INVALID_HID) {
    return bsp_construct_default_array_t();
  }

  hsize_t dims[3];

  int r = H5Sget_simple_extent_dims(fspace, dims, NULL);

  if (r < 0) {
    return bsp_construct_default_array_t();
  }

  hid_t hdf5_type = H5Dget_type(dset);

  bsp_type_t type = bsp_get_bsp_type(hdf5_type);

  bsp_array_t array = bsp_construct_array_t(dims[0], type);

  herr_t status = H5Dread(dset, bsp_get_hdf5_native_type(type), H5S_ALL,
                          H5S_ALL, H5P_DEFAULT, array.data);

  if (status < 0) {
    return bsp_construct_default_array_t();
  }

  H5Dclose(dset);
  H5Sclose(fspace);
  return array;
}

static inline void bsp_write_attribute(hid_t f, const char* label,
                                       const char* string) {
  hid_t strtype = H5Tcopy(H5T_C_S1);
  H5Tset_size(strtype, strlen(string));
  H5Tset_cset(strtype, H5T_CSET_UTF8);
  hid_t dataspace = H5Screate(H5S_SCALAR);

  hid_t attribute =
      H5Acreate2(f, label, strtype, dataspace, H5P_DEFAULT, H5P_DEFAULT);

  H5Awrite(attribute, strtype, string);

  H5Tclose(strtype);
  H5Aclose(attribute);
  H5Sclose(dataspace);
}

static inline char* bsp_read_attribute(hid_t f, const char* label) {
  hid_t attribute = H5Aopen(f, label, H5P_DEFAULT);
  hid_t strtype = H5Aget_type(attribute);

  hid_t type_class = H5Tget_class(strtype);
  assert(type_class == H5T_STRING);

  size_t size = H5Tget_size(strtype);

  char* string = (char*) malloc(size + 1);

  H5Aread(attribute, strtype, string);

  H5Aclose(attribute);
  H5Tclose(strtype);

  return string;
}
