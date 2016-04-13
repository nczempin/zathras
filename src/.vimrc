set tabstop=2
set softtabstop=2
set shiftwidth=2
set noexpandtab
set colorcolumn=81
set autoindent
set cindent
set textwidth=80
highlight ColorColumn ctermbg=blue  ctermfg=237 guibg=#990000
highlight OverLength ctermbg=red ctermfg=green guibg=#592929
match OverLength /\%81v.*/
augroup project
    autocmd!
    autocmd BufRead,BufNewFile *.h,*.c set filetype=c.doxygen
augroup END
nmap <F2> :w<CR>
map <F7> :make<CR>
map <F8> :!./ls<CR>
