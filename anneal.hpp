class Anneal {
private:
    double temperature;

public:
    Anneal (double temperature);
    double get_temperature(int iteration);
    double get_accept_threshold(double difference, int iteration);

};
