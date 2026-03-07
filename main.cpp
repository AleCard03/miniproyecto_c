#include <iostream>
#include "Eigen/Dense"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;
using namespace Eigen;

//En el csv hay valores con ',', los convierte a double
double stringToDouble(string s){
    for(int i = 0; i< s.length(); i++){
        if(s[i] == ','){
            s[i] = '.';
        }
    }
    return stod(s);    
}

int main()
{
    vector<string> names;
    vector<vector<double>> datos;
    string filename = "./EjemploEstudiantes.csv";
    ifstream file(filename);
    string line;

    //Este getline es para saltarnos la primera linea que contiene las materias
    getline(file,line);

    while(getline(file,line)){
        stringstream inputStream(line);
        string name, valor;
        vector<double> datosRow;
        getline(inputStream, name, ';');
        names.push_back(name);
        while(getline(inputStream,valor,';')){
            datosRow.push_back(stringToDouble(valor));
        }
        datos.push_back(datosRow);
        cout <<name << ": ";
        for(int i = 0; i<datosRow.size(); i++){
            cout << datosRow.at(i) << " ";
        }
        cout << endl;
    }

    int n = datos.size();
    int m = datos[0].size();

    //MatrixXd es de la Eigen library - la X representa que es de dimensiones dinámicas y la d que es un valor double
    //Se llena la matriz con los valores cargados del csv
    MatrixXd X(n,m);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            X(i,j) = datos[i][j];
        }
    }

    //Paso 1 - Centrar la Matriz (X - mu)
    VectorXd medias = X.colwise().mean();
    MatrixXd XCentrada = X.rowwise() - medias.transpose();

    //Estandarizar la Matriz (Z = XCentrada / sigma)
    //Calculamos manualmente dividiendo la norma cuadrada entre (n-1)
    VectorXd std_dev(m);
    for(int j = 0; j < m; j++) {
        std_dev(j) = sqrt(XCentrada.col(j).squaredNorm() / (n - 1));
    }

    MatrixXd Z(n, m);
    for(int j = 0; j < m; j++) {
        Z.col(j) = XCentrada.col(j).array() / std_dev(j);
    }

    //Paso 2 - Calcular la Matriz de Covarianza
    MatrixXd R = (Z.transpose() * Z) / (n - 1);

    //Paso 3 y 4 - Calcular Eigenvectors y Eigenvalues
    SelfAdjointEigenSolver<MatrixXd> solver(R);
    if (solver.info() != Success) {
        cerr << "Fallo al calcular los eigenvalores." << endl;
        return 1;
    }

    //Eigen devuelve los valores en orden ascendente. 
    VectorXd eigenvals_asc = solver.eigenvalues();
    MatrixXd eigenvecs_asc = solver.eigenvectors();

    VectorXd eigenvals_desc = eigenvals_asc.reverse();
    MatrixXd eigenvecs_desc = eigenvecs_asc.rowwise().reverse();

    //Paso 5 - Proyeccion - Z equivale a C en el codigo de Python
    MatrixXd Z_proyectada = Z * eigenvecs_desc;

    //Paso 6 - Calculo de Matriz de Calidades
    //distancias_estudiantes es un vector con la suma de Z squared por filas
    VectorXd distancias_estudiantes = Z.rowwise().squaredNorm();
    MatrixXd Q = Z_proyectada.array().square().colwise() / distancias_estudiantes.array();

    //Paso 7 - Calcular la matriz de coordenada de las variables
    MatrixXd T = eigenvecs_desc.array().rowwise() * eigenvals_desc.transpose().array().sqrt();

    //Paso 8 - Calcular la matriz de calidades de las variables
    //T_squared es un vector con la suma de Z squared por filas
    VectorXd T_squared = T.rowwise().squaredNorm();
    MatrixXd S = T.array().square().colwise() / T_squared.array(); 

    //Paso 9 - Calcular el vector de inerccias
    VectorXd varianza_explicada = eigenvals_desc.array() / eigenvals_desc.sum();
    VectorXd inercia_acumulada(varianza_explicada.size());
    double acumulado = 0.0;
    for (int i = 0; i < varianza_explicada.size(); ++i) {
        acumulado += varianza_explicada(i);
        inercia_acumulada(i) = acumulado;
    }
    printf("Varianza explicada por cada componente:\n");
    for (int i = 0; i < varianza_explicada.size(); ++i) {
        printf("Componente %d: %.4f%%\n", i + 1, varianza_explicada(i) * 100);
    }
    printf("\nInercia acumulada:\n");
    for (int i = 0; i < inercia_acumulada.size(); ++i) {
        printf("Componente %d: %.4f%%\n", i + 1, inercia_acumulada(i) * 100);
    } 

}