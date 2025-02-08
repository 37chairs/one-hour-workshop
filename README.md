# One Hour Workshop

## editor requirements

- install tmux, neovim

## nvimrc

```bash
# edit ~/.zshrc and add an alias to our tmux_esp.sh script
alias vesp="~/code/one-hour-workshop/tmux_esp.sh"

# to edit an espidf project, run
% vesp <path-to-project>
```

## tmux

1. Ctrl-b, B will run the build command
2. Ctrl-b, F will run the flash command
