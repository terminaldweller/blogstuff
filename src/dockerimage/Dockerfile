FROM alpine:3.12
RUN apk update && apk upgrade
RUN apk add tmux st vim ripgrep fzf curl wget git ctags zsh python3 lynx go make cmake gcc lua gdb

RUN mkdir devi
WORKDIR /devi

RUN git clone http://github.com/nanotech/jellybeans.vim ~/.vim/bundle/jellybeans.vim
RUN git clone http://github.com/VundleVim/Vundle.vim.git ~/.vim/bundle/Vundle.vim
RUN git clone http://github.com/tpope/vim-surround ~/.vim/bundle/vim-surround
RUN git clone http://github.com/easymotion/vim-easymotion ~/.vim/bundle/vim-easymotion
RUN git clone http://github.com/ctrlpvim/ctrlp.vim ~/.vim/bundle/ctrlp.vim
RUN git clone http://github.com/chrisbra/Recover.vim ~/.vim/bundle/Recover.vim
RUN git clone http://github.com/unblevable/quick-scope ~/.vim/bundle/quick-scope
RUN git clone http://github.com/tpope/vim-repeat ~/.vim/bundle/vim-repeat
RUN git clone http://github.com/adelarsq/vim-matchit ~/.vim/bundle/vim-matchit
RUN git clone http://github.com/majutsushi/tagbar ~/.vim/bundle/tagbar
RUN git clone http://github.com/ludovicchabant/vim-gutentags ~/.vim/bundle/vim-gutentags
RUN git clone http://github.com/mbbill/undotree ~/.vim/bundle/undotree

RUN git clone http://github.com/terminaldweller/fedora
RUN (cd fedora/baseline && cp ./.vimrc ~/.vimrc)
RUN (cd fedora/baseline && cp ./.tmux.conf ~/.tmux.conf)
RUN go get github.com/yudai/gotty

RUN sh -c "$(curl -fsSL https://raw.github.com/robbyrussell/oh-my-zsh/master/tools/install.sh)"
RUN (cd fedora/baseline && cp ./.zshrc ~/.zshrc)

RUN zsh
ENV SHELL=/bin/zsh
RUN (cd ~; git clone https://github.com/wting/autojump)
RUN (cd ~/autojump; python3 ./install.py)
RUN echo "#autojump" >> ~/.zshrc
RUN (cd ~; echo "[[ -s $(pwd)/.autojump/etc/profile.d/autojump.sh ]] && source $(pwd)/.autojump/etc/profile.d/autojump.sh" >> ~/.zshrc)
RUN echo "autoload -U compinit && compinit -u" >> ~/.zshrc
RUN (cd ~/.oh-my-zsh/plugins; git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ~/.oh-my-zsh/plugins/zsh-syntax-highlighting)

RUN vim -T xterm +Helptags +qall

RUN apk add openssh w3m torsocks mksh
RUN ln -s /usr/bin/python3 /bin/python
RUN ln -s /bin/mksh /bin/ksh
RUN mkdir ~/.w3m
RUN (cd fedora/.w3m && cp * ~/.w3m)
# RUN service tor start
EXPOSE 9000
# ENV TERM=st-256
# RUN st
