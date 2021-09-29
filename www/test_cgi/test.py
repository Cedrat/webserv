#!/usr/bin/python

import cgi

print 'Content-type: text/html'
print

print '<html><head><title>Mon super site</title></head><body>'
formulaire = cgi.FieldStorage()
if formulaire.getvalue('message') != None:
    print '<p>Merci d\'avoir particip&amp;eacute; au livre d\'or. Vous pouvez le visiter <a href="livreor.py">ici</a>.</p>'
    message = formulaire.getvalue('message')
    site = formulaire.getvalue('site')
    pseudo = formulaire.getvalue('pseudo')
    try:
        fichier = open('livreor','r')
        livreor = fichier.read()
    except IOError:
        livreor = ''
    message_poste = '<message><auteur>'+cgi.escape(pseudo)+'</auteur><site>'+cgi.escape(site)+'</site><contenu>'+cgi.escape(message)+'</contenu></message>\n'
    fichier = open('livreor','w')
    fichier.write(message_poste+livreor)
else:
    print '''Erreur : vous n'avez pas rempli le formulaire.'''
print '</body></html>'