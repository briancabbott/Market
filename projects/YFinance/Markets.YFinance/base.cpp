
#include <algorithm>
#include <map>
#include <string>

using namespace std;

class TickerBase {
public:

    TickerBase(string ticker, void* session, void* proxy) {
        transform(ticker.begin(), ticker.end(), ticker.begin(), ::toupper);
        this->ticker = ticker;
		this->session = session;
		this->proxy = proxy;
    }

	void _lazy_load_price_history() {
	}
	
	void _get_ticker_tz() {
	}
	
	void _fetch_ticker_tz() {
	}
	
	void get_recommendations(void* proxy = nullptr, bool as_dict = false) {
	}

	void get_recommendations_summary(void* proxy = nullptr, bool as_dict = false) {
	}

	void get_upgrades_downgrades(void* proxy = nullptr, bool as_dict = false) {
	}

	map<void*,void*> get_calendar(void* proxy = nullptr) {
		return {};
	}

	map<void*, void*> get_sec_filings(void* proxy = nullptr) {
		return {};
	}
	void get_major_holders(void* proxy = nullptr, bool as_dict = false) {
	}
	void get_institutional_holders(void* proxy = nullptr, bool as_dict = false) {
	}
	void get_mutualfund_holders(void* proxy = nullptr, bool as_dict = false) {
	}
	void get_insider_purchases(void* proxy = nullptr, bool as_dict = false) {
	}
	void get_insider_transactions(void* proxy = nullptr, bool as_dict = false) {
	}
	void get_insider_roster_holders(void* proxy = nullptr, bool as_dict = false) {
	}
	map<void*, void*> get_info(void* proxy = nullptr) {
		return {};
	}
	void get_fast_info(void* proxy = nullptr) {
	}
	void basic_info() {
	}
	void get_sustainability(void* proxy = nullptr, bool as_dict = false) {
	}

	map<void*, void*> get_analyst_price_targets(void* proxy = nullptr) {
		return {};
	}
	void get_earnings_estimate(void* proxy = nullptr, bool as_dict = false) {
	}
	void get_revenue_estimate(void* proxy = nullptr, bool as_dict = false) {
	}
	void get_earnings_history(void* proxy = nullptr, bool as_dict = false) {
	}
	void get_eps_trend(void* proxy = nullptr, bool as_dict = false) {
	}
	void get_eps_revisions(void* proxy = nullptr, bool as_dict = false) {
	}
	void get_growth_estimates(void* proxy = nullptr, bool as_dict = false) {
	}
	void get_earnings(void* proxy = nullptr, bool as_dict = false, string freq = "yearly") {
	}
	void get_income_stmt(void* proxy = nullptr, bool as_dict = false, bool pretty = false, string freq = "yearly") {
	}
	void get_incomestmt(void* proxy = nullptr, bool as_dict = false, bool pretty = false, string freq = "yearly") {
	}
	void get_financials(void* proxy = nullptr, bool as_dict = false, bool pretty = false, string freq = "yearly") {
	}
	void get_balance_sheet(void* proxy = nullptr, bool as_dict = false, bool pretty = false, string freq = "yearly") {
	}
	void get_balancesheet(void* proxy = nullptr, bool as_dict = false, bool pretty = false, string freq = "yearly") {
	}

	map<void*, void*> get_cash_flow(void* proxy = nullptr, bool as_dict = false, bool pretty = false, string freq = "yearly") {
		return {};
	}

	void get_cashflow(void* proxy = nullptr, bool as_dict = false, bool pretty = false, string freq = "yearly") {
	}
	
	//->pd.Series
	void get_dividends(void* proxy = nullptr) {
	}
	//->pd.Series
	void get_capital_gains(void* proxy = nullptr) {}
	void get_splits(void* proxy = nullptr) {}
	void get_actions(void* proxy = nullptr) {}
	void get_shares(void* proxy = nullptr, bool as_dict = false) {}
	void get_shares_full(void* start = nullptr, void* end = nullptr, void* proxy = nullptr) {}
	void get_isin(void* proxy = nullptr) {}
	void get_news(void* proxy = nullptr) {}
	void get_earnings_dates(int limit = 12, void* proxy = nullptr) {}
	void get_history_metadata(void* proxy = nullptr) {}
	void get_funds_data(void* proxy = nullptr) {}



private:
    string ticker;
    void* session;
    void* proxy;
	//   _tz;
	//_isin;
	//_news;
	//_shares;
	//_earnings_dates;
	//_earnings;
	//_financials;
	//_data;
	//_price_history;
	//_analysis;
	//_holders;
	//_quote;
	//_fundamentals;
	//_funds_data;
	//_fast_info;

};
