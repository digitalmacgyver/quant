all: dvi ps pdf html

dvi:
	latex 473proj.tex
	latex 473proj.tex

ps:
	dvips -o 473proj.ps 473proj.dvi

pdf:
	dvipdf 473proj.dvi 473proj.pdf

html:
	mkdir -p 473proj
	latex2html -dir 473proj -local_icons -html_version 3.2,math -info 0 -address "Matthew Hayward - <a href=\"http://github.com/digitalmacgyver/quant/tree/master/473/\">Quantum Computing and Grover's Algorithm GitHub Repository</a>" -bottom_navigation 473proj.tex

clean:
	rm -rf 473proj/
	rm -f 473proj.dvi
	rm -f 473proj.aux
