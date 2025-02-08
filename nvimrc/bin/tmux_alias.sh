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
    # Create new session and start Neovim in the given directory
    tmux new-session -d -s $SESSION -n editor -c "$PROJECT_DIR" "nvim -u ~/code/one-hour-workshop/nvimrc/init.lua"

    # Create a new terminal pane in the same directory
    tmux split-window -h -t $SESSION -c "$PROJECT_DIR" "source ~/esp/esp-idf/export.sh && source ~/esp/esp-idf/python_env/idf5.4_py3.13_env/bin/activate; exec bash"
    tmux send-keys -t $SESSION "clear" C-m

    # Resize the terminal pane to 40% width
    tmux resize-pane -t $SESSION -x 40%

    # Set key bindings inside tmux
    tmux bind-key B send-keys -t $SESSION:0.1 C-u "idf.py build" C-m
    tmux bind-key F send-keys -t $SESSION:0.1 C-u "idf.py flash monitor" C-m
fi

# Attach to session
tmux attach -t $SESSION
