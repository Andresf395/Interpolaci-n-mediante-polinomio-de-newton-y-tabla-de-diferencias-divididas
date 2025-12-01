#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>
#include <limits>
#include <cmath>
using namespace std;

// Lee valores separados por comas o punto y coma, acepta varias líneas hasta completar n valores
vector<double> leerValoresComas(int n) {
    vector<double> valores;
    valores.reserve(n);
    string linea;

    while ((int)valores.size() < n) {
        if (!getline(cin, linea)) break; // EOF o error

        // ignorar líneas vacías
        if (linea.find_first_not_of(" \t\r\n") == string::npos) continue;

        // reemplazar ';' por ',' para unificar separadores
        replace(linea.begin(), linea.end(), ';', ',');

        stringstream ss(linea);
        string token;
        while (getline(ss, token, ',')) {
            // eliminar espacios en blanco alrededor
            token.erase(remove_if(token.begin(), token.end(), [](unsigned char c){ return isspace(c); }), token.end());
            if (token.empty()) continue;
            try {
                double v = stod(token);
                valores.push_back(v);
                if ((int)valores.size() == n) break;
            } catch (const invalid_argument&) {
                cerr << "Aviso: token no numérico ignorado: '" << token << "'\n";
            } catch (const out_of_range&) {
                cerr << "Aviso: token fuera de rango ignorado: '" << token << "'\n";
            }
        }

        if ((int)valores.size() < n) {
            cout << "Se han leido " << valores.size() << " de " << n << " valores. Ingrese más (separados por comas):\n";
        }
    }

    return valores;
}

// Calcular la tabla de diferencias divididas y mostrarla
vector<double> diferenciasDivididas(const vector<double>& x, const vector<double>& y) {
    int n = x.size();
    vector<vector<double>> tabla(n, vector<double>(n, 0.0));

    for (int i = 0; i < n; ++i) tabla[i][0] = y[i];

    for (int j = 1; j < n; ++j) {
        for (int i = 0; i < n - j; ++i) {
            double denom = x[i + j] - x[i];
            if (denom == 0.0) {
                cerr << "Error: dos abscisas iguales (x[" << i << "] == x[" << i + j << "]) -> división por cero.\n";
                exit(1);
            }
            tabla[i][j] = (tabla[i + 1][j - 1] - tabla[i][j - 1]) / denom;
        }
    }

    cout << "\nTabla de diferencias divididas:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n - i; ++j) {
            cout << setw(14) << tabla[i][j];
        }
        cout << '\n';
    }

    vector<double> coef(n);
    for (int i = 0; i < n; ++i) coef[i] = tabla[0][i];
    return coef;
}

// Construir representación simbólica del polinomio (forma de Newton)
string construirPolinomio(const vector<double>& x, const vector<double>& coef) {
    ostringstream poly;
    poly << fixed << setprecision(6);
    poly << "P(x) = ";

    int m = coef.size();
    for (int i = 0; i < m; ++i) {
        double a = coef[i];
        if (i > 0) {
            if (a >= 0) poly << " + ";
            else { poly << " - "; a = -a; }
        } else {
            // para el primer término, manejar signo negativo
            if (a < 0) { poly << "-"; a = -a; }
        }

        poly << a;

        for (int j = 0; j < i; ++j) {
            poly << "*(x";
            if (x[j] >= 0) poly << " - " << x[j] << ")";
            else poly << " + " << -x[j] << ")";
        }
    }

    return poly.str();
}

// Evaluar el polinomio de Newton en x_val
double evaluarNewton(const vector<double>& x, const vector<double>& coef, double x_val) {
    int m = coef.size();
    double resultado = 0.0;
    // Evaluación eficiente (Horner para forma Newton)
    // resultado = coef[m-1]; luego ir acumulando
    resultado = coef[m-1];
    for (int i = m - 2; i >= 0; --i) {
        // en forma de Newton versión adaptada:
        // pero la iteración "clásica" es acumulando productos:
        // mejor usar la forma directa (segura) usada antes
    }

    // Usamos la versión directa (más clara):
    resultado = coef[0];
    double producto = 1.0;
    for (int i = 1; i < m; ++i) {
        producto *= (x_val - x[i-1]);
        resultado += coef[i] * producto;
    }
    return resultado;
}

int main() {
    cout << setprecision(8);
    int n;
    cout << "Ingrese el numero de puntos (entero > 0): ";
    if (!(cin >> n) || n <= 0) {
        cerr << "Entrada invalida para n.\n";
        return 1;
    }

    // limpiar buffer para usar getline en leerValoresComas
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Ingrese los valores de x separados por comas (puede usar varias lineas si hace falta):\n";
    vector<double> x = leerValoresComas(n);
    if ((int)x.size() != n) {
        cerr << "No se leyeron suficientes valores de x. Saliendo.\n";
        return 1;
    }

    cout << "Ingrese los valores de y separados por comas (puede usar varias lineas si hace falta):\n";
    vector<double> y = leerValoresComas(n);
    if ((int)y.size() != n) {
        cerr << "No se leyeron suficientes valores de y. Saliendo.\n";
        return 1;
    }

    // Mostrar pares por comprobacion
    cout << "\nPares (x,y):\n";
    for (int i = 0; i < n; ++i) cout << " (" << x[i] << ", " << y[i] << ")\n";

    vector<double> coef = diferenciasDivididas(x, y);

    cout << "\nCoeficientes del polinomio de Newton:\n";
    for (int i = 0; i < (int)coef.size(); ++i) cout << "a" << i << " = " << coef[i] << '\n';

    cout << "\nForma simbolica del polinomio:\n" << construirPolinomio(x, coef) << '\n';

    // Opcion para interpolar o no
    while (true) {
        cout << "\n¿Desea interpolar un nuevo valor? (s/n): ";
        string resp;
        if (! (cin >> resp)) break;
        if (resp.empty()) continue;
        char c = resp[0];
        if (c == 'n' || c == 'N') break;
        if (c == 's' || c == 'S') {
            double xv;
            cout << "Ingrese el valor de x a interpolar: ";
            if (!(cin >> xv)) {
                cerr << "Entrada invalida para x.\n";
                // limpiar estado y buffer
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            double r = evaluarNewton(x, coef, xv);
            cout << "P(" << xv << ") = " << r << '\n';
        } else {
            cout << "Respuesta no reconocida, use 's' o 'n'.\n";
        }
    }

    cout << "\nPrograma finalizado.\n";
    system("pause");
    return 0;
}
