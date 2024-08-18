#include <iostream>
#include <fstream>

#include "include/ServerHandler.hpp"


int generate_key_and_cert() {
     // Инициализация OpenSSL
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    // Генерация RSA ключа
    RSA *rsa = RSA_generate_key(2048, RSA_F4, nullptr, nullptr);
    
    // Создание EVP_PKEY и установка RSA ключа
    EVP_PKEY *pkey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(pkey, rsa);

    // Сохранение закрытого ключа в PEM файл
    FILE *privateKeyFile = fopen("private_key.pem", "wb");
    PEM_write_PrivateKey(privateKeyFile, pkey, nullptr, nullptr, 0, nullptr, nullptr);
    fclose(privateKeyFile);

    // Создание сертификата
    X509 *x509 = X509_new();
    ASN1_INTEGER_set(X509_get_serialNumber(x509), 1); // Установка серийного номера
    X509_gmtime_adj(X509_get_notBefore(x509), 0); // Установка даты начала действия
    X509_gmtime_adj(X509_get_notAfter(x509), 31536000L); // Установка даты окончания действия (1 год)

    // Установка публичного ключа
    X509_set_pubkey(x509, pkey);

    // Подписание сертификата
    X509_sign(x509, pkey, EVP_sha256());

    // Сохранение сертификата в PEM файл
    FILE *certFile = fopen("certificate.pem", "wb");
    PEM_write_X509(certFile, x509);
    fclose(certFile);

    // Освобождение ресурсов
    X509_free(x509);
    EVP_PKEY_free(pkey);
    ERR_free_strings();
}


int generate_dh()
{
	int keySize = 2048;

    // Создание нового объекта DH
    DH *dh = DH_new();
    if (!dh) {
        std::cerr << "Ошибка при создании объекта DH." << std::endl;
        return 1;
    }

    // Генерация параметров DH
    if (DH_generate_parameters_ex(dh, keySize, DH_GENERATOR_2, nullptr) != 1) {
        std::cerr << "Ошибка при генерации параметров DH." << std::endl;
        DH_free(dh);
        return 1;
    }

    // Генерация ключей
    if (DH_generate_key(dh) != 1) {
        std::cerr << "Ошибка при генерации ключей DH." << std::endl;
        DH_free(dh);
        return 1;
    }

    // Открытие файла для записи параметров
    FILE *file = fopen("dhparams.pem", "w");
    if (!file) {
        std::cerr << "Не удалось открыть файл для записи." << std::endl;
        DH_free(dh);
        return 1;
    }

    // Запись параметров в файл
    if (PEM_write_DHparams(file, dh) != 1) {
        std::cerr << "Ошибка при записи параметров DH в файл." << std::endl;
        fclose(file);
        DH_free(dh);
        return 1;
    }

    // Закрытие файла и освобождение ресурсов
    fclose(file);
    DH_free(dh);
	return 0;
}


int main()
{
    generate_key_and_cert();
	generate_dh();
	database::DatabaseClient* dbClient = new database::DatabaseClient;
	j_parser::Parser* Parser = new j_parser::Parser;
	server::Server server(8080, "localhost", dbClient, Parser);
}