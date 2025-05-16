#include "alg.h"

using namespace std;


std::string Ai[] =
    {
        "Очень морозно",
        "Обычная зимняя погода",
        "Тепло"
    };
std::string Bi[] =
    {
        "Высокая влажность",
        "Средняя влажность",
        "Низкая влажность"

    };
std::string Ci[] =
    {
        "Очень холодно и сыро",
        "Холодно с умеренной сыростью",
        "Холодно и сухо",
        "Умеренно и сыро",
        "Комфортная погода",
        "Умеренно и сухо",
        "Жарко и душно",
        "Тепло и комфортно",
        "Тепло и сухо"
    };

// Функция принадлежности
double triangular(double x, double a, double b) {
    if (x < min(a, b) || x > max(a, b))
        return 0.0;
    return (x - b) / (a - b);
}


double mu_A1(double x) { return triangular(x, -30, -15); }
double mu_A2(double x) { return triangular(x, -20, -5); }
double mu_A3(double x) { return triangular(x, 0, -10); }

double mu_B1(double x) { return triangular(x, 100, 55); }
double mu_B2(double x) { return triangular(x, 66, 25); }
double mu_B3(double x) { return triangular(x, 35, 0); }

vector<function<double(double)>> C = {
    [](double y){ return triangular(y, 40, 0); },
    [](double y){ return triangular(y, 45, 25); },
    [](double y){ return triangular(y, 50, 30); },
    [](double y){ return triangular(y, 55, 35); },
    [](double y){ return triangular(y, 60, 40); },
    [](double y){ return triangular(y, 65, 45); },
    [](double y){ return triangular(y, 80, 60); },
    [](double y){ return triangular(y, 85, 65); },
    [](double y){ return triangular(y, 100, 70); }
};

// Мамдани или Ларсен: min или prod
void fuzzy_inference(double temp, double hum, struct result* res, bool useMamdani)
{
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

            res -> phasificationA[i] = alpha1;
            res -> phasificationB[j] = alpha2;
            res -> aggregation[i * 3 + j] = activation[i * 3 + j];
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

    res -> y0 = (denominator == 0) ? 0 : numerator / denominator; // дефаззификация


    // Найти терм C[i], которому принадлежит y0 наиболее сильно
    int bestIndex = 0;
    double maxMu = 0.0;

    for (int r = 0; r < lenC; ++r) {
        double mu = C[r](res -> y0); // степень принадлежности y0
        if (mu > maxMu) {
            maxMu = mu;
            bestIndex = r;
        }
    }
    res -> C = Ci[bestIndex];
}

