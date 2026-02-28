#include <iostream>
#include "Eigen/Dense"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;
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
}