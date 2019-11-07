NAME=index

TEX_FILES = $(shell find . -name '*.tex')
CLS_FILES = $(shell find . -name '*.cls')
LST_FILES = $(shell find . -name '*.lst')
BIB_FILES = $(shell find . -name '*.bib')

.PHONY: all
all: $(NAME).pdf

.PHONY: clean
clean:
	latexmk -c
	find . -type f -name '*.aux' -delete
	find . -type f -name '*.bbl' -delete
	rm -f $(NAME).acn
	rm -f $(NAME).acr
	rm -f $(NAME).alg
	rm -f $(NAME).glg
	rm -f $(NAME).glo
	rm -f $(NAME).gls
	rm -f $(NAME).glsdefs
	rm -f $(NAME).ist
	rm -f $(NAME).loa

.PHONY: purge
purge: clean
	latexmk -C

.PHONY: view
view: $(NAME).pdf
	okular $(NAME).pdf >/dev/null 2>&1 &

$(NAME).pdf: $(TEX_FILES) $(CLS_FILES) $(LST_FILES) $(BIB_FILES)
	latexmk -pdf $(NAME).tex
