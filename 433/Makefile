all: dvi ps pdf html

dvi:
	latex shor-par.tex
	latex shor-par.tex

ps:
	dvips -o shor-par.ps shor-par.dvi

pdf:
	dvipdf shor-par.dvi shor-par.pdf

html:
	mkdir -p shor-par
	latex2html -dir shor-par -local_icons -html_version 3.2,math -info 0 -bottom_navigation -address "Matthew Hayward - <a href=\"http://github.com/digitalmacgyver/quant/tree/master/433/\">Quantum Computing, Shor's Algorithm, and Parallelism GitHub Repository</a>" shor-par.tex

clean:
	rm -rf shor-par/
	rm -f shor-par.dvi
	rm -f shor-par.aux

