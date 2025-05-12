#include "alg.h"

using namespace std;

// Функция принадлежности
double triangular(double x, double a, double b) {
    if (x < min(a, b) || x > max(a, b))
        return 0.0;
    return (x - b) / (a - b);
}


// Температура Ai
double mu_A1(double x) { return triangular(x, -30, -15); }
double mu_A2(double x) { return triangular(x, -20, -5); }
double mu_A3(double x) { return triangular(x, -10, 0); }

// Влажность Bi
double mu_B1(double x) { return triangular(x, 100, 55); }
double mu_B2(double x) { return triangular(x, 66, 25); }
double mu_B3(double x) { return triangular(x, 35, 0); }

// Выходные C1-C9
vector<function<double(double)>> C = {
    [](double y){ return triangular(y, 40, 0); }, // C1
    [](double y){ return triangular(y, 45, 25); }, // C2
    [](double y){ return triangular(y, 50, 30); }, // C3
    [](double y){ return triangular(y, 55, 35); }, // C4
    [](double y){ return triangular(y, 60, 40); }, // C5
    [](double y){ return triangular(y, 65, 45); }, // C6
    [](double y){ return triangular(y, 80, 60); }, // C7
    [](double y){ return triangular(y, 85, 65); }, // C8
    [](double y){ return triangular(y, 100, 70); }  // C9
};

// Мамдани или Ларсен: min или prod
double fuzzy_inference(double temp, double hum, bool useMamdani) {
    vector<function<double(double)>> mu_A = {mu_A1, mu_A2, mu_A3};
    vector<function<double(double)>> mu_B = {mu_B1, mu_B2, mu_B3};

    vector<double> activation(9);

    // Шаг 1-2: фаззификация и агрегирование (α = min / prod)
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) {
            double alpha1 = mu_A[i](temp); // μAi(x1*)
            double alpha2 = mu_B[j](hum);  // μBj(x2*)
            activation[i * 3 + j] = useMamdani ? min(alpha1, alpha2)
                                               : (alpha1 * alpha2); // агрегирование (отсечение)
        }

    // Шаги 3–5: активизация → аккумуляция → дефаззификация
    double numerator = 0.0, denominator = 0.0;

    for (int k = 0; k < N; ++k) {
        double y = X_MIN + k * (X_MAX - X_MIN) / (N - 1);
        double mu = 0.0;

        for (int r = 0; r < 9; ++r) {
            double muC = C[r](y);
            double val = useMamdani ? min(activation[r], muC) // Мамдани: μC* = α ∧ μC
                                    : activation[r] * muC;    // Ларсен: μC* = α × μC
            mu = max(mu, val); // аккумуляция
        }

        numerator += y * mu;
        denominator += mu;
    }

    return (denominator == 0) ? 0 : numerator / denominator; // дефаззификация
}

