#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cmath>  
using namespace std;

// Usamos long double para mayor precisión
typedef long double ldouble;

// Calcular la tabla de diferencias divididas
vector<ldouble> diferenciasDivididas(const vector<ldouble>& x, const vector<ldouble>& y) {
    int n = x.size();
    vector<vector<ldouble>> tabla(n, vector<ldouble>(n));

    // Primera columna: valores de y
    for (int i = 0; i < n; ++i)
        tabla[i][0] = y[i];

    // Cálculo de diferencias divididas
    for (int j = 1; j < n; ++j) {
        for (int i = 0; i < n - j; ++i) {
            tabla[i][j] = (tabla[i + 1][j - 1] - tabla[i][j - 1]) / (x[i + j] - x[i]);
        }
    }

    // Extraer coeficientes
    vector<ldouble> coef(n);
    for (int i = 0; i < n; ++i)
        coef[i] = tabla[0][i];

    return coef;
}

// Construir el polinomio en forma simbólica (Newton)
string construirPolinomio(const vector<ldouble>& x, const vector<ldouble>& coef) {
    stringstream poly;
    poly << fixed << setprecision(10); // ajusta la precisión mostrada

    poly << "P(x) = ";

    for (int i = 0; i < coef.size(); ++i) {
        if (i > 0) {
            poly << (coef[i] >= 0 ? " + " : " - ");
            poly << abs(coef[i]);
        } else {
            poly << coef[i];
        }

        for (int j = 0; j < i; ++j) {
            poly << "(x - " << x[j] << ")";
        }
    }

    return poly.str();
}

// Evaluar el polinomio de Newton en un punto x_val
ldouble evaluarNewton(const vector<ldouble>& x, const vector<ldouble>& coef, ldouble x_val) {
    ldouble resultado = coef[0];
    ldouble producto = 1.0;

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

    vector<ldouble> x(n), y(n);
    cout << "Ingrese los valores de x:\n";
    for (int i = 0; i < n; ++i)
        cin >> x[i];

    cout << "Ingrese los valores de y:\n";
    for (int i = 0; i < n; ++i)
        cin >> y[i];

    // Calculamos los coeficientes
    vector<ldouble> coef = diferenciasDivididas(x, y);

    cout << fixed << setprecision(15);  // Mostrar decimales largos

    cout << "\nCoeficientes del polinomio de Newton:\n";
    for (int i = 0; i < n; ++i)
        cout << "a" << i << " = " << coef[i] << endl;

    cout << "\nForma simbólica del polinomio:\n";
    cout << construirPolinomio(x, coef) << endl;

    char opcion;
    do {
        cout << "\n¿Desea interpolar un nuevo valor de x? (s/n): ";
        cin >> opcion;

        if (opcion == 's' || opcion == 'S') {
            ldouble x_val;
            cout << "Ingrese el valor de x a evaluar: ";
            cin >> x_val;
            ldouble resultado = evaluarNewton(x, coef, x_val);
            cout << "P(" << x_val << ") = " << resultado << endl;
        }

    } while (opcion == 's' || opcion == 'S');

    cout << "\nPrograma finalizado." << endl;
    system ("pause");
    return 0;
    
}
