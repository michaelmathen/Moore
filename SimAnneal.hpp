#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
using std::vector;


#define INITIAL_ACCEPT .9
#define ACCEPT_THRESHOLD 1e-7
#define SIMULATED_ANNEAL_P 1.0

double x_hat(double T, vector<double> &emax, vector<double> &emin, int sample_size) {
  //Calculate the next predicted probability of acceptance based on the current probability
  int i;
  double sum_num = 0;
  double sum_denom = 0;
  for (i = 0; i < sample_size; i++){
    sum_num += exp(-emax[i] / T);
    sum_denom += exp(-emin[i] / T);
  }
  return sum_num / sum_denom;
}

template<int MT>
void calculate_move_costs(MooreGraph<MT> &g, vector<double> &s0Costs,
		vector<double> &s1Costs, int sample_size) {
	//This is an iterative method from Computing the Initial Temperature of Simulated
	//Annealing by Walid Ben-Ameur look at the bottom of page 4
	s0Costs.resize(sample_size);
	s1Costs.resize(sample_size);
	auto state1 = g;
	for (int i = 0; i < sample_size;) {
		//Get the costs associated with random moves
		state1.randomize();
		s0Costs[i] = state1.heuristicScore();
		auto delta = state1.random_delta();
		state1.apply_delta(delta);
		s1Costs[i] = state1.heuristicScore();
		//Only moves with higher costs
		if (s0Costs[i] < s1Costs[i])
			i++;
	}
}

double probability_to_temperature(vector<double> &s0Costs, vector<double> &s1Costs,
								  double desired_prob){
	int sample_size = s0Costs.size();
	//Need to calculate a good starting point for our method
	double delta_sum = 0.0;
	for (int i = 0; i < sample_size; i++) {
		delta_sum += s1Costs[i] - s0Costs[i];
	}
	double x_Hat;
	double t_nplus1;
	double t_n;
	//Do this iteration once just to set some values
	double t_nminus1 = -delta_sum / (sample_size * log(sample_size));

	double p = SIMULATED_ANNEAL_P;
	x_Hat = x_hat(t_nminus1, s0Costs, s1Costs, sample_size);
	t_n = -t_nminus1 * pow(log(x_Hat) / log(desired_prob), 1 / p);

	while (1) {
		x_Hat = x_hat(t_n, s0Costs, s1Costs, sample_size);
		if (abs(x_Hat - desired_prob) <= ACCEPT_THRESHOLD)
			return t_n;
		t_nplus1 = -t_n * pow(log(x_Hat) / log(desired_prob), 1. / p);

		if ((t_nplus1 - t_n) * (t_n - t_nminus1) < 0.0) {
			//If there is oscillation in the temperature then we need a larger value for p
			p = 2 * p;
		}
		//begin next iteration
		t_nminus1 = t_n;
		t_n = t_nplus1;
	}
	return 0.0;
}

template<int MT>
double temperature_initial(MooreGraph<MT> &g, int sample_size, double desired_prob) {
	vector<double> s0Costs;
	vector<double> s1Costs;
	calculate_move_costs<MT>(g, s0Costs, s1Costs, sample_size);
	return probability_to_temperature(s0Costs, s1Costs, desired_prob);
}

double acceptance_probability(double old_e, double new_e, double t){
  if (new_e < old_e)
    return 1;
  else
    return exp(-(new_e - old_e) / t);
}

double temperature_cauchy(double i, double t0){
  double tr = t0 /(1.0 + i);
  return tr;
}


double temperature_boltzmann(double i, double t0){
  return t0 / log(1 + i);
}

template<int MT>
MooreGraph<MT> simmulated_anneal(MooreGraph<MT> &moore_g, int max_iterations){
  //initialize the random number generator
  srand48(time(NULL));
  srand(time(NULL));

  //Allocate space for some temporary states
  auto current_state = moore_g;
  auto best_state = moore_g;

  //Evaluate the initial energy level
  double current_energy = (double)moore_g.heuristicScore();
  double best_energy = current_energy;
  double t0 = temperature_initial(moore_g, 100, .8);
  std::cout << "Initial temperature: " << t0 << std::endl;
  //Iterate for a maximum number of iterations
  int i = 0;
  double j = 0;
  while(i < max_iterations){

    double t_i = temperature_boltzmann(j, t0);
    j += 1.0;
    auto delta = current_state.random_delta();
    current_state.apply_delta(delta);

    double new_energy = current_state.heuristicScore();

    if (acceptance_probability(current_energy, new_energy, t_i) > drand48()) {
    	//std::cout << "Current temperature " << t_i << std::endl;
		  current_energy = new_energy;
    } else {
    	current_state.apply_delta(delta);
    }
    if (current_energy == 0){
    	return current_state;
    }
    if (current_energy < best_energy){
    	i = 0;
    	best_energy = new_energy;
    	best_state = current_state;
    	std::cout << std::endl;
    	current_state.printHeuristicMatrix();
    }
    i++;
  }

  return best_state;
}

