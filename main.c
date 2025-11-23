#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double f(double x) {
    return 1.0 / (4.0 * x * x + x + 2.0);
}

double integral_left(double a, double b, unsigned int n) {
    double h = (b - a) / n;
    double sum = 0.0;
    for (unsigned int i = 0; i < n; ++i) {
        double x = a + i * h;
        sum += f(x);
    }
    return sum * h;
}

double integral_right(double a, double b, unsigned int n) {
    double h = (b - a) / n;
    double sum = 0.0;
    for (unsigned int i = 1; i <= n; ++i) {
        double x = a + i * h;
        sum += f(x);
    }
    return sum * h;
}

double integral_trapezoid(double a, double b, unsigned int n) {
    double h = (b - a) / n;
    double sum = 0.5 * (f(a) + f(b));
    for (unsigned int i = 1; i < n; ++i) {
        double x = a + i * h;
        sum += f(x);
    }
    return sum * h;
}

double integral_simpson(double a, double b, unsigned int n) {
    if (n % 2 != 0) n++;
    double h = (b - a) / n;
    double sum = f(a) + f(b);
    for (unsigned int i = 1; i < n; ++i) {
        double x = a + i * h;
        sum += f(x) * (i % 2 == 0 ? 2.0 : 4.0);
    }
    return sum * h / 3.0;
}

double refine_integral(double (*method)(double, double, unsigned int), double a, double b, double eps, unsigned int *final_n) {
    unsigned int n = 4;
    double I1 = method(a, b, n);
    double I2;
    do {
        n += 2;
        I2 = method(a, b, n);
    } while (fabs(I2 - I1) > eps);
    *final_n = n;
    return I2;
}

int main(void) {
    double a = 0.0, b = 1.0, eps = 0.0001;
    unsigned int n = 0, method = 0;

    printf("Vvedit mezhi integruvannya [a, b]:\n");
    printf("a = "); scanf("%lf", &a);
    printf("b = "); scanf("%lf", &b);

    printf("Vvedit kilkist promizhkiv (N > 0):\n");
    scanf("%u", &n);
    if (n == 0) {
        printf("ERROR: N maye buty > 0\n");
        return 1;
    }

    printf("Vvedit pohybku epsilon (napryklad 0.0001):\n");
    scanf("%lf", &eps);
    if (eps < 0.00001 || eps > 0.001) {
        printf("ERROR: ε maye buty v mezhah [0.00001, 0.001]\n");
        return 1;
    }

    printf("\nOberit metod:\n");
    printf("1 Livi pryamokutnyky\n");
    printf("2 Pravi pryamokutnyky\n");
    printf("3 Trapecii\n");
    printf("4 Paraboly (Sympson)\n");
    scanf("%u", &method);

    double result = 0.0;
    switch (method) {
        case 1:
            result = integral_left(a, b, n);
            break;
        case 2:
            result = integral_right(a, b, n);
            break;
        case 3:
            result = integral_trapezoid(a, b, n);
            break;
        case 4:
            result = integral_simpson(a, b, n);
            break;
        default:
            printf("Nevirnyy vybir metodu\n");
            return 1;
    }

    printf("\nResultat obchyslennya:\n");
    printf("Integral = %.8lf\n", result);
    printf("N = %u\n", n);


    unsigned int refined_n = 0;
    double refined_result = refine_integral(
        method == 1 ? integral_left :
        method == 2 ? integral_right :
        method == 3 ? integral_trapezoid :
        integral_simpson,
        a, b, eps, &refined_n
    );

    printf("\nUtochnenyy resultat:\n");
    printf("Integral = %.8lf\n", refined_result);
    printf("N = %u\n", refined_n);
    printf("Delta = %.8lf\n", fabs(refined_result - result));

    return 0;
}
