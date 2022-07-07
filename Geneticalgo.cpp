#include<iostream>
#include<vector>	
#include<fstream>
#include<sstream>
#include <string>
#include<time.h>
#include<math.h>
#include <utility>

using namespace std;
vector<bool> selection();
void theInput(ifstream& fi, vector<double>&, vector<double>&, vector<double>&);
vector<double>Lnormalization(int size, vector<double>theUtlty, vector<double>the_weight);
pair<vector<bool>, vector<bool>>cross_overP(int size, vector<double>& fitnss, vector<double>& pounds);
pair<vector<bool>, vector<bool>>mutation(double proba, int Size, vector<double>Fitness, vector<double>Pounds);
double fitness(vector<bool> genome, vector<double> Utlty, vector<double>Weight);

int main() {

    ifstream infile;
    ofstream oufile;
    vector<double> v, utility, weight;
    pair<vector<bool>, vector<bool>> p;
    int pop_size;
    double prob = 0.0001, Fitness = 0;

    //open my original file 
    infile.open("Pinput.txt");

    if (infile.fail()) {
        cout << "file did not open";
    }
    theInput(infile, v, utility, weight);
    oufile.open("output.txt");
    cin >> pop_size;
    oufile << "Starting population " << pop_size << endl;

    for (int i = 0; i < 20; i++) {

        p = mutation(prob, pop_size, utility, weight);

        Fitness += fitness(p.first, utility, weight);
    }
    oufile << "Max fitness after 5000 generation:" << p.first[1];



}

//created a vector that contain my file elements also create 2 vectors each of utility and weight.
void theInput(ifstream& file, vector<double>& vect, vector<double>& vect2, vector<double>& vect3) {

    string s, word;
    istringstream iss;
    while (!file.eof())
    {
        getline(file, s);
        if (file.good()) {
            iss.clear();
            iss.str(s);
            while (iss.good()) {
                iss >> word;

                vect.push_back(stod(word));

            }
        }
    }
    for (int i = 0; i < vect.size(); i++) {

        if (i % 2 == 0)
            vect2.push_back(vect[i]);
        else vect3.push_back(vect[i]);
    }
}

//function that make a random selection. 
vector<bool>selection() {

    vector<bool> selected(400);
    srand(time(0));
    int random_number;
    int count = 0;

    while (count < 20) {
        random_number = (rand() % 400);
        selected[random_number] = 1;
        count++;
    }

    for (int i = 0; i < 400; i++) {
        if (selected[i] != 1)
            selected[i] = 0;
    }
    return selected;
}
//function that generate an initial population 
vector <vector<bool>> population(int size) {

    vector <vector<bool>> generation;

    for (int i = 0; i < size; i++) {


        generation.push_back(selection());
    }

    return generation;
}

//fitness function for each selection

double fitness(vector<bool> genome, vector<double> Utlty, vector<double>Weight) {

    double weight = 0;
    double utility = 0;
    for (int i = 0; i < genome.size(); i++) {

        if (genome[i] == 1)
        {
            weight += Weight[i];
            utility += Utlty[i];
            if (weight > 500) return 1;
        }
    }

    return utility;
}

//function that returns a vector of probability distrbution.
vector<double>Lnormalization(int size, vector<double>theUtlty, vector<double>the_weight) {

    //calculating the probability distribution using L2 
    double sum = 0;
    vector<double> Pvect, Lvect;
    vector<vector<bool>> gene = population(size), Nw_population;
    for (int i = 0; i < size; i++) {

        Pvect.push_back(pow(fitness(gene[i], theUtlty, the_weight), 2));
        sum += Pvect[i];
    }
    for (int i = 0; i < size; i++) {
        Lvect.push_back(Pvect[i] / sum);//Lvec contain the probability distribution for the initial population
    }
    return Lvect;

}

//return a pair of good selection based on the probability distribution
pair<vector<bool>, vector<bool>> genome_pair(int size, vector<double>fitnss, vector<double>tweight) {
    srand(time(0));
    double r;
    vector<vector<bool>> generation = population(size);
    vector<double> probability = Lnormalization(size, fitnss, tweight);
    pair<vector<bool>, vector<bool>> parents;

    while (parents.first.empty() && parents.second.empty()) {
        r = rand() / RAND_MAX;
        for (int i = 0; i < probability.size(); i++) {

            if (r < probability[i]) {
                if (parents.first.empty()) {
                    parents.first = generation[i];
                    break;
                }
                else {
                    parents.second = generation[i];
                    break;
                }
            }
        }
    }
    return parents;
}


// cross over pair 
pair<vector<bool>, vector<bool>>cross_overP(int size, vector<double>& fitnss, vector<double>& pounds) {
    srand(time(0));
    pair<vector<bool>, vector<bool>> passed_Paired = genome_pair(size, fitnss, pounds);

    int random = rand() % 400;// crossover point
    for (int i = random; i < 400; i++)
    {
        bool temp;

        temp = passed_Paired.first[i];
        passed_Paired.first[i] = passed_Paired.second[i];
        passed_Paired.second[i] = temp;
    }
    return passed_Paired;
}
// this function will return a mutated pair
pair<vector<bool>, vector<bool>>mutation(double proba, int Size, vector<double>Fitness, vector<double>Pounds)
{
    srand(time(0));
    double R;
    R = rand() / RAND_MAX;
    pair<vector<bool>, vector<bool>> vector_pair = cross_overP(Size, Fitness, Pounds);

    for (int i = 0; i < 400; i++) {

        if (R < proba)
        {
            if (vector_pair.first[i] == 1)
                vector_pair.first[i] == 0;
            else vector_pair.first[i] == 1;
            if (vector_pair.second[i] == 1)
                vector_pair.second[i] == 0;
            else vector_pair.second[i] == 1;

        }
    }
    return vector_pair;
}









