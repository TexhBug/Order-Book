#include <bits/stdc++.h>
using namespace std;

unordered_map <float, pair<unordered_map<string, int>, int>> BUY; 
// This will hold all the buy orders
unordered_map <float, pair<unordered_map<string, int>, int>> SELL; 
// This will hold all the sell orders

priority_queue <float> BID; 
// Max heap for Bids(Buy)
priority_queue <float, vector<float>, greater<float>> ASK; 
// Min heap for Asks(Sell)

unordered_map<string, int> Orders;
// FOr tracking existing orders in order book

float current_price;
float spread;

void start() {
    current_price = 50.20;
    spread = 0.10;
}

void share_current_price() {
    cout<<current_price<<endl;
}

void add_order(string id, float price, int quantity, string type) {
    if(type == "BUY") {
        if(BUY.find(price) != BUY.end()) {
            BUY[price].first[id] = quantity;
            BUY[price].second += quantity;
        }
        else {
            BUY[price] = {{{id, quantity}}, quantity};
            BID.push(price);
        }
    }
    else {
        if(SELL.find(price) != SELL.end()) {
            SELL[price].first[id] = quantity;
            SELL[price].second += quantity;
        }
        else {
            SELL[price] = {{{id, quantity}}, quantity};
            ASK.push(price);
        }
    }
    Orders[id]++;
}

void cancel_order(string id, float price, int quantity, string type) {
    if(type == "BUY") {
        BUY[price].second -= BUY[price].first[id];
        BUY[price].first.erase(id);
    }
    else {
        SELL[price].second -= SELL[price].first[id];
        SELL[price].first.erase(id);
    }
    Orders.erase(id);
}

void update_order(string id, float old_price, int old_quantity, string old_type, float new_price, int new_quantity, string new_type) {
    if(Orders.find(id) != Orders.end()) {
        cancel_order(id, old_price, old_quantity, old_type);
        add_order(id, new_price, new_quantity, new_type);
    }
    else{
        cout<<"Cannot process the updates as order does not exist in order book - either executed or cancelled"<<endl;
    }
}

void execute_market_order(string id, int quantity, string position) {
    if (position == "BUY") {
        while (!ASK.empty() && quantity > 0) {
            float ask = ASK.top();
            for (auto i = SELL[ask].first.begin(); i != SELL[ask].first.end();) {
                if(i->second <= quantity) {
                    quantity -= i->second;
                    SELL[ask].second -= i->second;
                    Orders.erase(i->first);
                    i = SELL[ask].first.erase(i);
                }
                else {
                    i->second -= quantity;
                    SELL[ask].second -= quantity;
                    quantity = 0;
                }
                if(quantity == 0) break;
            }
            if(quantity > 0 || (quantity == 0 && SELL[ask].second == 0)) {
                ASK.pop();
                SELL.erase(ask);
            }
            current_price = ask;
            cout<<"Current Price"<<endl;
            share_current_price();
        }
        if(quantity != 0) {
            cout<<"Cancelling the remaining shares "<<quantity<<" in the order"<<endl;
        }
    }
    else {
        while (!BID.empty() && quantity > 0) {
            float bid = BID.top();
            for (auto i = BUY[bid].first.begin(); i != BUY[bid].first.end();) {
                if(i->second <= quantity) {
                    quantity -= i->second;
                    BUY[bid].second -= i->second;
                    Orders.erase(i->first);
                    i = BUY[bid].first.erase(i); // This return the next valid iterator in the map
                }
                else {
                    i->second -= quantity;
                    BUY[bid].second -= quantity;
                    quantity = 0;
                }
                if(quantity == 0) break;
            }
            if(quantity > 0 || (quantity == 0 && BUY[bid].second == 0)) {
                BID.pop();
                BUY.erase(bid);
            }
            current_price = bid;
            cout<<"Current Price"<<endl;
            share_current_price();
        }
        if(quantity != 0) {
            cout<<"Cancelling the remaining shares "<<quantity<<" in the order"<<endl;
        }
    }
}

void execute_limit_order(string id, float price, int quantity, string position) {
    if(position == "BUY") {
        if (price > current_price + spread) {
            cout<<"Error : For BUY limit orders price should be lower than or equal to current price + spread of the share"<<endl;
        }
        else {
            if(!ASK.empty() && ASK.top() == price) {
                float ask = ASK.top();
                for (auto i = SELL[ask].first.begin(); i != SELL[ask].first.end();) {
                    if(i->second <= quantity) {
                        quantity -= i->second;
                        SELL[ask].second -= i->second;
                        Orders.erase(i->first);
                        i = SELL[ask].first.erase(i);
                    }
                    else {
                        i->second -= quantity;
                        SELL[ask].second -= quantity;
                        quantity = 0;
                    }
                    if(quantity == 0) break;
                }
                if(quantity > 0) {
                    ASK.pop();
                    SELL.erase(ask);
                    cout<<"Adding "<<quantity<<" in order book"<<endl;
                    add_order(id, price, quantity, position);
                }
                else if(quantity == 0 && SELL[ask].second == 0) {
                    ASK.pop();
                    SELL.erase(ask);
                }
                current_price = price;
                cout<<"Current Price"<<endl;
                share_current_price();
            }
            else {
                cout<<"Adding "<<quantity<<" in order book"<<endl;
                add_order(id, price, quantity, position);
            }
        }
    }
    else {
        if (price < current_price - spread) {
            cout<<"Error : For SELL limit orders price should be greater than or equal to current price - spread of the share"<<endl;
        }
        else {
            if(!BID.empty() && BID.top() == price) {
                float bid = BID.top();
                for (auto i = BUY[bid].first.begin(); i != BUY[bid].first.end();) {
                    if(i->second <= quantity) {
                        quantity -= i->second;
                        BUY[bid].second -= i->second;
                        Orders.erase(i->first);
                        i = BUY[bid].first.erase(i);
                    }
                    else {
                        i->second -= quantity;
                        BUY[bid].second -= quantity;
                        quantity = 0;
                    }
                    if(quantity == 0) break;
                }
                if(quantity > 0) {
                    BID.pop();
                    BUY.erase(bid);
                    cout<<"Adding "<<quantity<<" in order book"<<endl;
                    add_order(id, price, quantity, position);
                }
                else if(quantity == 0 && BUY[bid].second == 0) {
                    BID.pop();
                    BUY.erase(bid);
                }
                current_price = price;
                cout<<"Current Price"<<endl;
                share_current_price();
            }
            else {
                cout<<"Adding "<<quantity<<" in order book"<<endl;
                add_order(id, price, quantity, position);
            }
        }
    }
    
}

string generate_id() {
    long long id = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::system_clock::now().time_since_epoch())
                       .count();
    return std::to_string(id); // Convert long long to string
}

int random_quantity() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 100);
    return dis(gen);
}

double random_price(double base_price) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(-2, 2); // Generate integers between -10 and 10

    // Generate a random step in multiples of 0.05
    double step = dis(gen) * 0.05;
    double price = base_price + step;

    // Round to 2 decimal places (ensures no floating-point precision issues)
    return round(price * 100.0) / 100.0;
}


void generate_random_order() {
    string id = generate_id();
    string position = rand() % 2 == 0 ? "BUY" : "SELL";
    string type = rand() % 2 == 0 ? "LIMIT" : "MARKET";
    int quantity = random_quantity();
    float price = random_price(current_price);

    if (type == "MARKET") {
        cout<<id<<" "<<quantity<<" "<<current_price<<" "<<position<<"  "<<type<< endl;
        execute_market_order(id, quantity, position);
    }
    else {
        cout<<id<<" "<<quantity<<" "<<price<<" "<<position<<" "<<type<<endl;
        execute_limit_order(id, price, quantity, position);
    }
}

int main() {
	start();
    for(int i = 0; i < 10; ++i){
        generate_random_order();
        this_thread::sleep_for(chrono::milliseconds(500));
        cout<<"-----------------------------------"<<endl;
    }
	
	cout<<"Hello";
	return 0;
	
}
