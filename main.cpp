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
        for(int j = 0; j < m; i++){
            X(i,j) = datos[i][j];
        }
    }

    //Centrar la Matriz (X - mu)
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

    //Calcular la Matriz de Covarianza
    MatrixXd R = (Z.transpose() * Z) / (n - 1);

    //Calcular Eigenvectors y Eigenvalues
    //Usamos SelfAdjointEigenSolver porque R es una matriz simétrica.
    SelfAdjointEigenSolver<MatrixXd> solver(R);
    if (solver.info() != Success) {
        cerr << "Fallo al calcular los eigenvalores." << endl;
        return 1;
    }

    //Eigen devuelve los valores en orden ascendente. 
    VectorXd eigenvals_asc = solver.eigenvalues();
    MatrixXd eigenvecs_asc = solver.eigenvectors();
}