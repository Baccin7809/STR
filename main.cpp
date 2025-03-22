#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
// code by @brunobaccin
enum Estado { PENSANDO, COM_FOME, COMENDO };

std::mutex garfos[5]; // Mutex para os garfos

// Função para mostrar o estado do sistema
void exibirEstados(const Estado estados[], const bool garfos[]) {
    for (int i = 0; i < 5; i++) {//loop que percore o vetor de estado 
        char estadoChar;//variavel do estado 
        if (estados[i] == PENSANDO) estadoChar = 'P';// se o estado for pensando o char recebe p
        else if (estados[i] == COM_FOME) estadoChar = 'F';
        else estadoChar = 'C';
        std::cout << estadoChar << ",";//printa o estado e uma virgula
    }
    std::cout << " | ";//printa um espaçamento

    for (int i = 0; i < 5; i++) {//loop pra percorrer os garfos
        std::cout << (garfos[i] ? "O," : "L,");//printa livre ou ocupado
    }
    std::cout << std::endl;//quebra de linha
}

// Função para o filósofo agir
void filosofo(int id, Estado estados[], bool garfos[], std::mutex& mtx) {
    while (true) {
        estados[id] = PENSANDO;// vetor de estado na posição do id do filosofo recebe string PENSANDO
        exibirEstados(estados, garfos);//printa todos os estados de garfos e filosofos
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //delay q simula o tempo da ação

        estados[id] = COM_FOME;// muda o estado do id recebido da função
        exibirEstados(estados, garfos);//printa novamente 

        std::lock_guard<std::mutex> lock(mtx);// bloqueia o acesso ao codigo abaixo caso outras threads tentem acessar

        int garfoEsq = id; // garfo esquerda na posição do filosofo
        int garfoDir = (id + 1) % 5;//o garfo a direita da posição do filosofo

        if (!garfos[garfoEsq] && !garfos[garfoDir]) { //ambos os garfos estarem false 
            garfos[garfoEsq] = true;//ativa os garfos como true
            garfos[garfoDir] = true;

            estados[id] = COMENDO;//muda o estado do filosofo para comendo
            exibirEstados(estados, garfos);// exibe  novamente
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));//delay pra simular o tempo de ação

            garfos[garfoEsq] = false;// apos comer os garfos retornam como false
            garfos[garfoDir] = false;

            estados[id] = PENSANDO;// filosofo volta a pensar
            exibirEstados(estados, garfos);//printa novamente os filosofos e garfos
        }
    }
}

int main() {
    std::mutex mtx;//definindo um mutex

    Estado estados[5] = {PENSANDO, PENSANDO, PENSANDO, PENSANDO, PENSANDO};// vetor de string
    bool garfos[5] = {false, false, false, false, false};// vetor de booleano para os 5 garfos 


    std::vector<std::thread> threads; // declaração de um vetor de threads

    for (int i = 0; i < 5; i++) {
        threads.push_back(std::thread(filosofo, i, std::ref(estados), std::ref(garfos), std::ref(mtx)));//cria a thread e coloca no vetor e faz a thread fazer a função filosofo
    }

    for (auto& t : threads) {// espera as threads terminarem
        t.join();
    }

    return 0;
}
