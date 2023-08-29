#include <iostream>
#include <string>
#include <random>
#include <chrono>

std::string gerarStringAleatoria(int tamanho, const std::string& caracteresPossiveis) {
    std::string stringAleatoria;
    std::random_device rd;
    std::mt19937 gerador(rd());
    std::uniform_int_distribution<int> distribuicao(0, caracteresPossiveis.size() - 1);

    for (int i = 0; i < tamanho; i++) {
        stringAleatoria += caracteresPossiveis[distribuicao(gerador)];
    }

    return stringAleatoria;
}

void exibirDesenho() {
    std::cout << "##################################################################################################################" << std::endl;
    std::cout << "##                                                                                                              ##" << std::endl;
    std::cout << "##  ######   ######  ##   ##  ######    #######                    #######   #####   ######     ####   #######  ##" << std::endl;
    std::cout << "##  ##  ##   ##  ##  ##   ##  # ## #    ##   #                     ##   #  ##   ##   ##  ##   ##  ##   ##   #   ##" << std::endl;
    std::cout << "##  ##  ##   ##  ##  ##   ##    ##      ## #                       ## #    ##   ##   ##  ##  ##        ## #     ##" << std::endl;
    std::cout << "##  #####    #####   ##   ##    ##      ####         #######       ####    ##   ##   #####   ##        ####     ##" << std::endl;
    std::cout << "##  ##  ##   ## ##   ##   ##    ##      ## #                       ## #    ##   ##   ## ##   ##        ## #     ##" << std::endl;
    std::cout << "##  ##  ##   ##  ##  ##   ##    ##      ##   #                     ##      ##   ##   ##  ##   ##  ##   ##   #   ##" << std::endl;
    std::cout << "##  ######   #### ## #######    ##      #######                    ####      #####   #### ##    ####   #######  ##" << std::endl;
    std::cout << "##                                                                                                              ##" << std::endl;
    std::cout << "##################################################################################################################" << std::endl;
}

int main() {
    std::string sequenciaAlvo = "a1b";
    std::string caracteresPossiveis = "abcdefghijklmnopqrstuvwxyz0123456789";
    
    auto inicio = std::chrono::high_resolution_clock::now();

    while (true) {
        std::system("cls");
        exibirDesenho();
        std::string sequenciaAleatoria = gerarStringAleatoria(sequenciaAlvo.length(), caracteresPossiveis);
        
        std::cout << " " << std::endl;
        std::cout << "Tentando sequencia : " << sequenciaAleatoria << std::endl;

        if (sequenciaAleatoria == sequenciaAlvo) {
            auto fim = std::chrono::high_resolution_clock::now();
            std::cout << "Sequencia encontrada: " << sequenciaAleatoria << std::endl;

            auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio);
            std::cout << "Tempo decorrido: " << duracao.count() << " ms" << std::endl;

            break;
        }
    }

    return 0;
}

