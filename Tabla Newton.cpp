#include <iostream>
#include <vector>
#include <iomanip>  // Para formatear la salida
#include <cstdlib>
using namespace std;

// Función que calcula las diferencias divididas
vector<double> diferenciasDivididas(const vector<double>& x, const vector<double>& y) {
    int n = x.size();
    vector<vector<double>> tabla(n, vector<double>(n));

    // Primera columna: los valores de y
    for (int i = 0; i < n; ++i)
        tabla[i][0] = y[i];

    // Construcción de la tabla de diferencias divididas
    for (int j = 1; j < n; ++j) {
        for (int i = 0; i < n - j; ++i) {
            tabla[i][j] = (tabla[i + 1][j - 1] - tabla[i][j - 1]) / (x[i + j] - x[i]);
        }
    }

    // Extraemos los coeficientes (primera fila de cada columna)
    vector<double> coef(n);
    for (int i = 0; i < n; ++i)
        coef[i] = tabla[0][i];

    //  Mostrar la tabla de diferencias
    cout << "\nTabla de diferencias divididas:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n - i; ++j)
            cout << setw(12) << tabla[i][j] << " ";
        cout << endl;
    }

    return coef;
}

// Función que evalúa el polinomio de Newton en un punto x_val
double evaluarNewton(const vector<double>& x, const vector<double>& coef, double x_val) {
    double resultado = coef[0];
    double producto = 1.0;

    for (int i = 1; i < x.size(); ++i) {
        producto *= (x_val - x[i - 1]);
        resultado += coef[i] * producto;
    }

    return resultado;
}

int main() {
    int n;
    cout << "Ingrese el numero de puntos: ";
    cin >> n;

    vector<double> x(n), y(n);
    cout << "Ingrese los valores de x:\n";
    for (int i = 0; i < n; ++i)
        cin >> x[i];

    cout << "Ingrese los valores de y:\n";
    for (int i = 0; i < n; ++i)
        cin >> y[i];

    // Calcular diferencias divididas
    vector<double> coef = diferenciasDivididas(x, y);

    // Mostrar coeficientes
    cout << "\nCoeficientes del polinomio de Newton:\n";
    for (int i = 0; i < n; ++i)
        cout << "a" << i << " = " << coef[i] << endl;

    // Evaluar el polinomio
    double x_val;
    cout << "\nIngrese el valor de x a interpolar: ";
    cin >> x_val;

    double resultado = evaluarNewton(x, coef, x_val);
    cout << "\nEl valor interpolado en x = " << x_val << " es: " << resultado << endl;
system ("pause");
    return 0;
}
