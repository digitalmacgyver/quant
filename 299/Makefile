all: dvi ps pdf html

dvi:
	latex paper.tex
	latex paper.tex

ps:
	dvips -o paper.ps paper.dvi

pdf:
	dvipdf paper.dvi paper.pdf

html:
	mkdir -p paper
	latex2html -dir paper -local_icons -html_version 3.2,math -info 0 -bottom_navigation -address "Matthew Hayward - <a href=\"http://github.com/digitalmacgyver/quant/tree/master/299/\">Quantum Computing and Shor's Algorithm GitHub Repository</a>" paper.tex

clean:
	rm -rf paper/
	rm -f paper.dvi
	rm -f paper.aux

