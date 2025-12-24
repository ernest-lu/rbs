#include <bits/stdc++.h>
using namespace std;

// clang-format off
template <typename T, typename = void> struct is_iterable : false_type {};template <typename T>struct is_iterable<    T, void_t<decltype(begin(declval<T>())), decltype(end(declval<T>()))>>    : true_type {};template <typename T>typename enable_if<is_iterable<T>::value && !is_same<T, string>::value,ostream &>::type operator<<(ostream &cout, T const &v);template <typename A, typename B>ostream &operator<<(ostream &cout, pair<A, B> const &p) {  return cout << "(" << p.first << ", " << p.second << ")";}template <typename T>typename enable_if<is_iterable<T>::value && !is_same<T, string>::value, ostream &>::type operator<<(ostream &cout, T const &v) {  cout << "[";  for (auto it = v.begin(); it != v.end();) {cout << *it; if (++it != v.end()) cout << ", ";  } return cout << "]";}

#ifdef LOCAL
  void dbg_out() { cout << endl; } template <typename Head, typename... Tail> void dbg_out(Head H, Tail... T) { cout << ' ' << H; dbg_out(T...); }
  #define debug(...) cout << "(" << #__VA_ARGS__ << "):", dbg_out(__VA_ARGS__)
#else
  #define debug(...) "zzz"
#endif
// clang-format on

int main() {
  constexpr int NUM_INPUT_VARIABLES = 10;
  constexpr int NUM_MONOMIALS = 1 << NUM_INPUT_VARIABLES;

  struct DataPoint {
    vector<double> inputs;
    double output;
  };

  auto load_dataset_from_file = [&](const std::string &filename,
                                    std::vector<DataPoint> &dataset) {
    ifstream file(filename);
    if (!file.is_open()) {
      cerr << "Error: Could not open dataset file '" << filename << "'" << endl;
      assert(false);
    }

    string line;
    while (getline(file, line)) {
      stringstream ss(line);
      string sequence;
      string bool_str;
      ss >> sequence >> bool_str;

      if (sequence.length() != NUM_INPUT_VARIABLES) {
        cerr << "Warning: Skipping malformed line: " << line << endl;
        continue;
      }

      DataPoint dp;
      dp.inputs.resize(NUM_INPUT_VARIABLES);
      for (int i = 0; i < NUM_INPUT_VARIABLES; ++i) {
        dp.inputs[i] = (sequence[i] == '(') ? 1.0 : 0.0;
      }

      dp.output = (bool_str == "True") ? 1.0 : 0.0;
      dataset.push_back(dp);
    }
    file.close();
  };

  auto calculate_all_monomials = [&](const vector<double> &inputs,
                                     vector<double> &monomial_values) {
    monomial_values.resize(NUM_MONOMIALS);
    monomial_values[0] = 1.0;
    for (int j = 0; j < NUM_INPUT_VARIABLES; ++j) {
      int power_of_2 = 1 << j;
      for (int i = 0; i < power_of_2; ++i) {
        monomial_values[i + power_of_2] = monomial_values[i] * inputs[j];
      }
    }
  };

  auto evaluate = [&](const vector<double> &weights,
                      const vector<double> &monomial_values) {
    double result = 0.0;
    for (int i = 0; i < NUM_MONOMIALS; ++i) {
      result += weights[i] * monomial_values[i];
    }
    return 1.0 / (1.0 + exp(-result));
  };

  const int EPOCHS = 1000;
  const double LEARNING_RATE = 0.07;
  const double BETA = 0.9;

  cout << "Loading dataset from 'bracket_sequences.txt'..." << endl;
  vector<DataPoint> dataset;
  load_dataset_from_file("bracket_sequences.txt", dataset);
  if (dataset.empty()) {
    cerr << "Dataset is empty or file could not be read. Exiting." << endl;
    return 1;
  }
  cout << "Dataset loaded with " << dataset.size() << " data points." << endl;

  // --- 3. Initialize model weights ---
  mt19937 rng(69); // Fixed seed for reproducibility
  uniform_real_distribution<double> uniform_dist(0.0, 1.0);
  vector<double> model_weights(NUM_MONOMIALS);
  for (int i = 0; i < NUM_MONOMIALS; ++i) {
    model_weights[i] = uniform_dist(rng);
  }
  vector<double> velocity(NUM_MONOMIALS, 0.0);

  // --- 4. Gradient Descent Optimization ---
  cout << "\nStarting optimization using Gradient Descent..." << endl;
  cout << fixed << setprecision(6);

  for (int epoch = 0; epoch < EPOCHS; ++epoch) {
    vector<double> gradient(NUM_MONOMIALS, 0.0);
    double mean_squared_error = 0.0;

    for (const auto &dp : dataset) {
      vector<double> monomial_values;
      calculate_all_monomials(dp.inputs, monomial_values);
      double prediction = evaluate(model_weights, monomial_values);
      prediction = round(prediction);
      double error = prediction - dp.output;
      mean_squared_error += error * error;
      for (int i = 0; i < NUM_MONOMIALS; ++i) {
        gradient[i] += error * monomial_values[i];
      }
    }

    mean_squared_error /= dataset.size();
    cout << "Epoch " << setw(3) << epoch + 1 << "/" << EPOCHS
         << ", MSE: " << mean_squared_error << endl;

    for (int i = 0; i < NUM_MONOMIALS; ++i) {
      double grad_component = 2.0 * gradient[i] / dataset.size();
      velocity[i] = BETA * velocity[i] - LEARNING_RATE * grad_component;
      model_weights[i] += velocity[i];
    }
  }

  debug(model_weights);

  vector<DataPoint> test_dataset;
  load_dataset_from_file("test_data.txt", test_dataset);

  cout << "\nOptimization finished." << endl;
  double final_mse = 0.0;
  for (const auto &dp : test_dataset) {
    vector<double> monomial_values;
    calculate_all_monomials(dp.inputs, monomial_values);
    double prediction = evaluate(model_weights, monomial_values);
    prediction = round(prediction);
    final_mse += pow(prediction - dp.output, 2);
  }
  final_mse /= dataset.size();
  cout << "Final MSE on dataset: " << final_mse << endl;

  return 0;
}
