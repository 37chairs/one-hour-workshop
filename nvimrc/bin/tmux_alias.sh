#!/bin/bash

SESSION="esp"
PROJECT_DIR="$1"

if [[ -z "$PROJECT_DIR" ]]; then
    echo "Usage: vesp <project-folder>"
    exit 1
fi

# Ensure the directory exists
if [[ ! -d "$PROJECT_DIR" ]]; then
    echo "Error: Directory '$PROJECT_DIR' does not exist."
    exit 1
fi

# Check if session exists
tmux has-session -t $SESSION 2>/dev/null

if [ $? != 0 ]; then
    # Create a new tmux session with Neovim in the left pane
    tmux new-session -d -s $SESSION -n editor -c "$PROJECT_DIR" "nvim -u ~/code/one-hour-workshop/nvimrc/init.lua"

    # Create a **right-side** terminal pane
    tmux split-window -h -t $SESSION -c "$PROJECT_DIR" \
        "source ~/esp/esp-idf/export.sh && source ~/esp/esp-idf/python_env/idf5.4_py3.13_env/bin/activate; exec bash"

    # Resize panes: Set left editor to ~85% width
    tmux select-pane -t $SESSION:0.0
    tmux resize-pane -R 85

    # Ensure Neovim pane is selected initially
    tmux select-pane -t $SESSION:0.0

    # Keybindings for resizing and swapping panes
    tmux bind-key -r '[' resize-pane -L 5  # Shrink editor
    tmux bind-key -r ']' resize-pane -R 5  # Expand editor
    tmux bind-key -r '\' swap-pane -s $SESSION:0.0 -t $SESSION:0.1 # Swap panes

    # ESP-IDF commands
    tmux bind-key B send-keys -t $SESSION:0.1 C-u "idf.py build" C-m
    tmux bind-key F send-keys -t $SESSION:0.1 C-u "idf.py flash monitor" C-m
    tmux bind-key Q send-keys -t $SESSION:0.1 C-u "tmux kill-session -t $SESSION" C-m

    # Git commands
    tmux bind-key G send-keys -t $SESSION:0.1 "git status" C-m
    tmux bind-key A send-keys -t $SESSION:0.1 "git add --patch" C-m
    tmux bind-key P send-keys -t $SESSION:0.1 "git push" C-m
    tmux bind-key C send-keys -t $SESSION:0.1 "git commit" C-m

fi

# Attach to session
tmux attach -t $SESSION
