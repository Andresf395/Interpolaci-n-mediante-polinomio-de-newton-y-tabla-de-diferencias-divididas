#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cstdlib>  // Para system("pause")
#include <cmath>    // Para abs()
using namespace std;

// Calcular la tabla de diferencias divididas
vector<double> diferenciasDivididas(const vector<double>& x, const vector<double>& y) {
    int n = x.size();
    vector<vector<double>> tabla(n, vector<double>(n));

    // Primera columna: valores de y
    for (int i = 0; i < n; ++i)
        tabla[i][0] = y[i];

    // Cálculo de diferencias divididas, la tablita 
    for (int j = 1; j < n; ++j) {
        for (int i = 0; i < n - j; ++i) {
            tabla[i][j] = (tabla[i + 1][j - 1] - tabla[i][j - 1]) / (x[i + j] - x[i]);
        }
    }

    // Extraer coeficientes
    vector<double> coef(n);
    for (int i = 0; i < n; ++i)
        coef[i] = tabla[0][i];

    return coef;
}

// Función para construir el polinomio en si, osea mostrarlo forma algebraica
string construirPolinomio(const vector<double>& x, const vector<double>& coef) {
    stringstream poly;
    poly << "P(x) = ";

    for (int i = 0; i < coef.size(); ++i) {
        // Mostrar signo
        if (i > 0 && coef[i] >= 0)
            poly << " + ";
        else if (coef[i] < 0)
            poly << " - ";

        // Valor absoluto del coeficiente
        poly << abs(coef[i]);

        // Factores (x - x_j)
        for (int j = 0; j < i; ++j) {
            poly << "(x";
            if (x[j] >= 0)
                poly << " - " << x[j] << ")";
            else
                poly << " + " << -x[j] << ")";
        }
    }

    return poly.str();
}

//Función que evalua el newton, similar a la tabla pero de manera indirecta
double evaluarNewton(const vector<double>& x, const vector<double>& coef, double x_val) {
    double resultado = coef[0];
    double producto = 1.0;

    for (int i = 1; i < x.size(); ++i) {
        producto *= (x_val - x[i - 1]);
        resultado += coef[i] * producto;
    }

    return resultado;
}

//Función core
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

    // Calcular coeficientes del polinomio de Newton
    vector<double> coef = diferenciasDivididas(x, y);

    // Mostrar coeficientes
    cout << "\nCoeficientes del polinomio de Newton:\n";
    for (int i = 0; i < n; ++i)
        cout << "a" << i << " = " << coef[i] << endl;

    // Mostrar polinomio simbólico
    cout << "\nForma simbólica del polinomio:\n";
    cout << construirPolinomio(x, coef) << endl;

    // Para interpolar valores en el rango
    char opcion;
    do {
        double x_val;
        cout << "\n¿Desea interpolar un nuevo valor de x? (s/n): ";
        cin >> opcion;

        if (opcion == 's' || opcion == 'S') {
            cout << "Ingrese el valor de x a evaluar: ";
            cin >> x_val;
            double resultado = evaluarNewton(x, coef, x_val);
            cout << "P(" << x_val << ") = " << resultado << endl;
        }

    } while (opcion == 's' || opcion == 'S');

    cout << "\nPrograma finalizado." << endl;
    system("pause");
    return 0;
}
