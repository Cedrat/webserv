<?php
$uploaddir = 'upload/';
$uploadfile = $uploaddir . basename($_FILES['userfile']['name']);

echo '<pre>';
if (move_uploaded_file($_FILES['userfile']['tmp_name'], $uploadfile)) {
    echo "Le fichier est valide, et a été téléchargé
           avec succès. Voici plus d'informations :\n";
} else {
    echo "Echec\n";
}

echo 'Voici quelques informations de débogage :';
print_r($_FILES);

echo '</pre>';

?>
