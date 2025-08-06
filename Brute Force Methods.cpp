#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include "picosha2.h" 

std::string gerarStringAleatoria(int tamanho, const std::string& caracteresPossiveis) {
    std::string stringAleatoria;
    static std::random_device rd;
    static std::mt19937 gerador(rd());
    std::uniform_int_distribution<int> distribuicao(0, caracteresPossiveis.size() - 1);

    for (int i = 0; i < tamanho; i++) {
        stringAleatoria += caracteresPossiveis[distribuicao(gerador)];
    }

    return stringAleatoria;
}

void exibirDesenho() {

    std::cout << "##################################################################################################################" << std::endl;
    std::cout << "##                                                                                                              ##" << std::endl;
    std::cout << "##  ######   ######    ##   ##   ######   #######                 #######  #######   ######    ####    #######  ##" << std::endl;
    std::cout << "##  ##  ##   ##  ##    ##   ##   # ## #   ##   #                  ##   #   ##   ##   ##  ##   ##  ##   ##   #   ##" << std::endl;
    std::cout << "##  ##  ##   ##  ##    ##   ##     ##     ## #                    ## #     ##   ##   ##  ##  ##        ## #     ##" << std::endl;
    std::cout << "##  #####    #####     ##   ##     ##     ####        #######     ####     ##   ##   #####   ##        ####     ##" << std::endl;
    std::cout << "##  ##  ##   ## ##     ##   ##     ##     ## #                    ## #     ##   ##   ## ##   ##        ## #     ##" << std::endl;
    std::cout << "##  ##  ##   ##  ##    ##   ##     ##     ##   #                  ##       ##   ##   ##  ##   ##  ##   ##   #   ##" << std::endl;
    std::cout << "##  ######   #### ##   #######     ##     #######                 ####     #######   #### ##   ####    #######  ##" << std::endl;
    std::cout << "##                                                                                                              ##" << std::endl;
    std::cout << "##################################################################################################################" << std::endl;

}
// ### MÉTODOS DE ATAQUE ###
// --- MÉTODO 1: FORÇA BRUTA EM TEXTO PLANO ---
void quebrarTextoPlano() {
    std::string sequenciaAlvo;
    std::string caracteresPossiveis = "abcdefghijklmnopqrstuvwxyz0123456789";

    std::cout << "\n--- Forca Bruta em Texto Plano ---\n";
    std::cout << "Digite a sequencia alvo (ex: abc, 123): ";
    std::cin >> sequenciaAlvo;

    long long tentativas = 0;
    auto inicio = std::chrono::high_resolution_clock::now();
    std::cout << "Procurando por '" << sequenciaAlvo << "'...\n" << std::endl;

    while (true) {
        std::string tentativa = gerarStringAleatoria(sequenciaAlvo.length(), caracteresPossiveis);
        tentativas++;

        if (tentativas % 1000 == 0) {
            std::cout << "\rTentando: " << tentativa << " | Tentativas: " << tentativas;
            std::cout.flush();
        }

        if (tentativa == sequenciaAlvo) {
            auto fim = std::chrono::high_resolution_clock::now();
            auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio);

            std::cout << "\r" << std::string(100, ' ') << "\r"; // Limpa a linha de status
            std::cout << "Sequencia encontrada!\n";
            std::cout << "Texto: " << tentativa << "\n";
            std::cout << "Total de tentativas: " << tentativas << "\n";
            std::cout << "Tempo decorrido: " << duracao.count() << " ms\n";
            break;
        }
    }
}
// --- MÉTODO 2: FORÇA BRUTA EM HASH SHA-256 ---
void quebrarHashSHA256() {
    std::string hashAlvo;
    int tamanhoDaSenha;
    std::string caracteresPossiveis = "abcdefghijklmnopqrstuvwxyz0123456789";

    std::cout << "\n--- Forca Bruta em Hash SHA-256 ---\n";
    std::cout << "Digite o hash SHA-256 alvo: ";
    std::cin >> hashAlvo;
    std::cout << "Digite o tamanho EXATO da senha original: ";
    std::cin >> tamanhoDaSenha;

    long long tentativas = 0;
    auto inicio = std::chrono::high_resolution_clock::now();
    std::cout << "Iniciando ataque no hash...\n" << std::endl;

    while (true) {
        std::string tentativa = gerarStringAleatoria(tamanhoDaSenha, caracteresPossiveis);
        std::string hashDaTentativa = picosha2::hash256_hex_string(tentativa);
        tentativas++;

        if (tentativas % 10000 == 0) { // Menos prints para mais velocidade
            std::cout << "\rTentando: " << tentativa << " | Hash: " << hashDaTentativa.substr(0, 10) << "... | Tentativas: " << tentativas;
            std::cout.flush();
        }

        if (hashDaTentativa == hashAlvo) {
            auto fim = std::chrono::high_resolution_clock::now();
            auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio);

            std::cout << "\r" << std::string(100, ' ') << "\r"; // Limpa a linha
            std::cout << "Hash Encontrado!\n";
            std::cout << "Texto Original: " << tentativa << "\n";
            std::cout << "Hash Correspondente: " << hashDaTentativa << "\n";
            std::cout << "Total de tentativas: " << tentativas << "\n";
            std::cout << "Tempo decorrido: " << duracao.count() << " ms\n";
            break;
        }
    }
}
// --- MÉTODO 3: FORÇA BRUTA TOTAL (INCREMENTAL) ---
void quebrarHashIncremental() {
    std::string hashAlvo;
    std::string caracteresPossiveis = "abcdefghijklmnopqrstuvwxyz0123456789";
    const int numCaracteres = caracteresPossiveis.length();

    std::cout << "\n--- Forca Bruta Total em Hash SHA-256 ---\n";
    std::cout << "ALERTA: Este metodo e EXTREMAMENTE lento para senhas com mais de 6-7 caracteres.\n";
    std::cout << "Digite o hash SHA-256 alvo: ";
    std::cin >> hashAlvo;

    long long tentativas = 0;
    auto inicio = std::chrono::high_resolution_clock::now();

    // Loop externo que controla o TAMANHO da senha
    for (int tamanho = 1; ; ++tamanho) {
        std::cout << "\nVerificando senhas de tamanho " << tamanho << "...\n";

        // 'indices' funciona como um contador em base 36. Cada elemento é um "dígito".
        std::vector<int> indices(tamanho, 0);

        // Loop interno que gera todas as combinações para o tamanho atual
        while (true) {
            std::string tentativa = "";
            for (int i = 0; i < tamanho; ++i) {
                tentativa += caracteresPossiveis[indices[i]];
            }

            // Calcula e compara o hash
            std::string hashDaTentativa = picosha2::hash256_hex_string(tentativa);
            tentativas++;

            if (hashDaTentativa == hashAlvo) {
                auto fim = std::chrono::high_resolution_clock::now();
                auto duracao = std::chrono::duration_cast<std::chrono::seconds>(fim - inicio);

                std::cout << "\r" << std::string(100, ' ') << "\r";
                std::cout << ">>> HASH ENCONTRADO! <<<\n";
                std::cout << "Texto Original: " << tentativa << "\n";
                std::cout << "Hash Correspondente: " << hashDaTentativa << "\n";
                std::cout << "Total de tentativas: " << tentativas << "\n";
                std::cout << "Tempo decorrido: " << duracao.count() << " segundos\n";
                return; // Encerra a função
            }

            if (tentativas % 500000 == 0) {
                std::cout << "\rTentando: " << tentativa << " | Tentativas: " << tentativas / 1000000 << "M";
                std::cout.flush();
            }

            // Lógica para "incrementar" o contador de índices
            int pos = tamanho - 1;
            while (pos >= 0) {
                indices[pos]++;
                if (indices[pos] < numCaracteres) {
                    break; // Incremento bem-sucedido, pode gerar a próxima string
                }
                // Se o "dígito" estourou (ex: de '9' para 'a' de novo), reseta e vai para o próximo "dígito" à esquerda
                indices[pos] = 0;
                pos--;
            }

            // Se pos < 0, significa que todos os dígitos estouraram (ex: "999").
            // Isso indica que todas as combinações para este tamanho foram testadas.
            if (pos < 0) {
                break; // Sai do loop interno para aumentar o tamanho da senha
            }
        }
    }
}
// ### FUNÇÃO PRINCIPAL COM O MENU ATUALIZADO ###
int main() {
    int escolha;

    while (true) {
#ifdef _WIN32
        std::system("cls");
#else
        std::system("clear");
#endif

        exibirDesenho();
        std::cout << "\n\nSelecione o modo de ataque:\n";
        std::cout << "1. Forca Bruta em Texto Plano (Adivinhar string)\n";
        std::cout << "2. Forca Bruta em Hash (comprimento conhecido)\n";
        std::cout << "3. Forca Bruta TOTAL em Hash (incremental, MUITO LENTO)\n";
        std::cout << "0. Sair\n";
        std::cout << "\nSua escolha: ";
        std::cin >> escolha;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            escolha = -1;
        }

        switch (escolha) {
        case 1:
            quebrarTextoPlano();
            break;
        case 2:
            quebrarHashSHA256();
            break;
        case 3:
            quebrarHashIncremental();
            break;
        case 0:
            std::cout << "Saindo do programa. Ate mais!\n";
            return 0;
        default:
            std::cout << "Opcao invalida! Tente novamente.\n";
            break;
        }

        std::cout << "\nPressione ENTER para voltar ao menu...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }

    return 0;
}