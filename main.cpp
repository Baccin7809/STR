#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

enum Estado { PENSANDO, COM_FOME, COMENDO };

std::mutex garfos[5]; // Mutex para os garfos

// Função para mostrar o estado do sistema
void exibirEstados(const Estado estados[], const bool garfos[]) {
    for (int i = 0; i < 5; i++) {
        char estadoChar;
        if (estados[i] == PENSANDO) estadoChar = 'P';
        else if (estados[i] == COM_FOME) estadoChar = 'F';
        else estadoChar = 'C';
        std::cout << estadoChar << ",";
    }
    std::cout << " | ";

    for (int i = 0; i < 5; i++) {
        std::cout << (garfos[i] ? "O," : "L,");
    }
    std::cout << std::endl;
}

// Função para o filósofo agir
void filosofo(int id, Estado estados[], bool garfos[], std::mutex& mtx) {
    while (true) {
        estados[id] = PENSANDO;
        exibirEstados(estados, garfos);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 

        estados[id] = COM_FOME;
        exibirEstados(estados, garfos);

        std::lock_guard<std::mutex> lock(mtx);

        int garfoEsq = id;
        int garfoDir = (id + 1) % 5;

        if (!garfos[garfoEsq] && !garfos[garfoDir]) {
            garfos[garfoEsq] = true;
            garfos[garfoDir] = true;

            estados[id] = COMENDO;
            exibirEstados(estados, garfos);
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));

            garfos[garfoEsq] = false;
            garfos[garfoDir] = false;

            estados[id] = PENSANDO;
            exibirEstados(estados, garfos);
        }
    }
}

int main() {
    std::mutex mtx;

    Estado estados[5] = {PENSANDO, PENSANDO, PENSANDO, PENSANDO, PENSANDO};
    bool garfos[5] = {false, false, false, false, false};

    std::vector<std::thread> threads;

    for (int i = 0; i < 5; i++) {
        threads.push_back(std::thread(filosofo, i, std::ref(estados), std::ref(garfos), std::ref(mtx)));
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
