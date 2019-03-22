#!/usr/local/bin/python
import sys, os

if len(sys.argv)==2:
	def parcours(rep):
		print("je suis dans : ",rep)
		liste=os.listdir(rep)
		for fichier in liste:
			if os.path.isdir(rep+"/"+fichier):
				parcours(rep+'/'+fichier)
			else:
				print("je compresse : ",fichier)
				os.system('./huf'+' '+fichier+' '+fichier+'huf')
	parcours(sys.argv[1])
else:
	print("Syntax: COMPRESSION REPERTOIRE : ", sys.arv[2])