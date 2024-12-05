#include <iostream>
#include <vector>

using namespace std;

vector<int> LFSR_ALT(const vector<int> &initial_state, int m, const vector<int> &feedback_taps, int output_length) {
    vector<int> new_state =  initial_state;
    vector<int> output_sequence;

    int wildbit = 0;
    for (int a = 0; a < output_length; ++a) {
        wildbit = new_state[feedback_taps[(feedback_taps.size()) - 1]] ^ new_state[feedback_taps[(feedback_taps.size()) - 2]];
        for (int i = static_cast<int>(feedback_taps.size()) - 3; i >= 0; i--) {
            wildbit = wildbit ^ new_state[feedback_taps[i]];
        }
        output_sequence.push_back(new_state.back());
        new_state.pop_back();
        new_state.insert(new_state.begin(), wildbit);
    }
    return output_sequence;
}

// Funkcja do generowania strumienia bitów z LFSR
vector<int> lfsr(vector<int> initial_state, vector<int> feedback_taps, int output_length) {
    vector<int> output_stream;
    int m = initial_state.size();  // długość rejestru

    // Główna pętla generowania strumienia bitów
    for (int i = 0; i < output_length; ++i) {
        // Pierwszy bit w rejestrze to wyjściowy bit
        int output_bit = initial_state[0];
        output_stream.push_back(output_bit);

        // Obliczanie bitu sprzężenia zwrotnego
        int feedback_bit = 0;
        for (int tap : feedback_taps) {
            feedback_bit ^= initial_state[tap];  // XOR na odpowiednich bitach
        }

        // Przesunięcie rejestru w lewo o jeden bit
        initial_state.erase(initial_state.begin());  // Usuwamy najstarszy bit
        initial_state.push_back(feedback_bit);  // Dodajemy obliczony bit sprzężenia zwrotnego na koniec
    }

    return output_stream;
}
/*
pair<vector<int>, int> Berlekamp_Massey(const vector<int>& stream) {
    vector<int> Connections_Polynomial(1);
    vector<int> Backup_Polynomial(1);
    int Linear_complexity = 0;
    int last_update_index = -1;
    int d = 0;
    int delta = 0;

    for (int N = 0; N < stream.size() - 1; ++N) {
        d = stream[N];
        for (int i = 1; i < Linear_complexity; ++i) {
            d = d^(Connections_Polynomial[i] * stream[N - i]);
        }
        if (d == 0) continue;
        vector<int>Temp_Polynomial = Connections_Polynomial;
        delta = N - last_update_index;
        for (int i = 0; i < Backup_Polynomial.size() - 1; ++i) {
            Connections_Polynomial[delta+i] = Connections_Polynomial[delta+i]^Backup_Polynomial[i];
        }
        if (2*last_update_index <= N) {
            Linear_complexity = N + 1 - Linear_complexity;
            Backup_Polynomial = Temp_Polynomial;
            last_update_index = N;
        }
    }
    pair<vector<int>, int> result;
    result.first = Connections_Polynomial;
    result.second = Linear_complexity;
    return result;
}
*/

pair<vector<int>, int> Berlekamp_Massey(const vector<int>& stream) {
    vector<int> Connections_Polynomial(1, 1);
    vector<int> Backup_Polynomial(1, 1);
    int Linear_complexity = 0;
    int last_update_index = -1;

    for (int N = 0; N < stream.size(); ++N) {
        int d = stream[N];
        for (int i = 1; i <= Linear_complexity; ++i) {
            d ^= Connections_Polynomial[i] * stream[N - i];
        }

        if (d == 0) continue;

        vector<int> Temp_Polynomial = Connections_Polynomial;
        int delta = N - last_update_index;

        if (Connections_Polynomial.size() < Backup_Polynomial.size() + delta) {
            Connections_Polynomial.resize(Backup_Polynomial.size() + delta, 0);
        }

        for (int i = 0; i < Backup_Polynomial.size(); ++i) {
            Connections_Polynomial[delta + i] ^= Backup_Polynomial[i];
        }

        if (2 * Linear_complexity <= N) {
            Linear_complexity = N + 1 - Linear_complexity;
            Backup_Polynomial = Temp_Polynomial;
            last_update_index = N;
        }
    }

    return {Connections_Polynomial, Linear_complexity};
}

int main() {

    //vector<int> initial_state = {1,0,1,0,1,1,0,0,1,1,1,0,0,0,0,1}; // Initial state of the register
    //vector<int> initial_state = {1,0,0}; // Initial state of the register
    //vector<int> initial_state = {0,1,0,0,1}; // Initial state of the register Zad2_1
    vector<int> initial_state = {1,0,1,1,0,1,0,1,1,0,1,1,0,1,0,1,0}; // Initial state of the register Zad1_L5
    int m = 5; // Degree of the LFSR
    //vector<int> feedback_taps = { 10, 13, 12, 15 };
    vector<int> feedback_taps = { 0, 4, 15, 16 };//Zad1_L5
    //vector<int> feedback_taps = { 1, 2 };//Zad 1
    //vector<int> feedback_taps = { 2, 4 };//Zad 2_1
    //vector<int> feedback_taps = {1, 3, 4 };//Zad 2_2
    //int output_length = 14;
    int output_length = 25;
    vector<int> output_stream = LFSR_ALT(initial_state, m, feedback_taps, output_length);
    /*
    // Stan początkowy rejestru (przykład: m = 3)
    vector<int> initial_state = { 0, 0, 1 };
    initial_state = {1,0,0,1,0}; // Initial state of the register Zad2_1

    // Połączenia sprzężenia zwrotnego (przykład: C(x) = 1 + x + x^3, więc feedback_taps = {0, 1})
    vector<int> feedback_taps = { 0, 1 };

    // Długość wyjściowego strumienia bitów
    int output_length = 14;

    // Generowanie strumienia bitów
    vector<int> output_stream = lfsr(initial_state, feedback_taps, output_length);
*/
    // Wyświetlenie wygenerowanego strumienia bitów
    for (int bit : output_stream) {
        cout << bit << " ";
    }
    cout << endl;

    pair<vector<int>, int> result = Berlekamp_Massey(output_stream);

    cout << "Result: " << endl;
    for (int bit : result.first) {
        cout << bit << " ";
    }
    cout << "Linear Complexity: " << result.second << endl;

    return 0;
}
