#ifndef CHUNKEDREQUEST_HPP
# define CHUNKEDREQUEST_HPP

#include "../includes/utils.hpp"

enum Info {incorrect_data, missing_data, good_data, all_data_read};

class ChunkedRequest
{
    private :
        std::string _data_not_processed;
        std::string _processed_data;

        int _nb_bytes_to_treat;

        void extractNbBytesToRead();
        Info checkNextDataIsByteToTreat();
        void treatData();


    public :
        ChunkedRequest();
        ~ChunkedRequest();
        void addData(std::string data);
        Info processData();
        void writeProcessedData(int fd);
};

#endif

/* 
    Lorsque les données sont récupérés. traiter les données possibles.
    Tant que toute les données actuelles n'ont pas été lues
        Si, la classe ne posséde pas de caractères en attente de traitements indiqué par _nb_bytes_to_treat;
            essayer d'extraire la valeur héxadecimale du nombre de bytes a traiter.
            Si echec car le bloc contenant la taille du nb de bytes a lire n'est pas entier
                attendre d'avoir plus de données.
            Si echec autre
                supprimer le fichier traité actuel
                preparer envoie header/body d'erreurs
            Si caractere de fin de chunked_request "\r\n0\r\n"
                Ajouter au fichier voulu le contenu de _processed_data;
                Passer a l'envoi des header/body.
        Sinon
            écrire le nb de caractères correspondant  dans un buffer temporaire _processed_data et passer_nb_bytes_to_treat;
            si _nb_bytes_to_treat est supérieur au nombre de caractères restants.
                Passer la valeur de _nb_bytes_to_treat au nombre de caracteres non lus nécessaire encore.
                ecrire le contenu de _processed_data dans le fd;
*/