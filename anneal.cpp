#include <math.h>
#include "anneal.hpp"

Anneal::Anneal(double start_temperature) {
    temperature = start_temperature;
}

double Anneal::get_temperature(int iteration) {
    return (temperature * (1.0 / log(iteration)));
}

double Anneal::get_accept_threshold(double difference, int iteration) {
    return (exp(difference / get_temperature(iteration)));
}
