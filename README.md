# One Hour Workshop

neovim setup and config for espidf development.  I borrowed some of the lua structure and defaults from [ThePrimeagen](https://github.com/ThePrimeagen).

## editor requirements

- install tmux, neovim

## nvimrc

TODO: add note re: installing  vim packages and language servers
Another test change

```bash
# edit ~/.zshrc and add an alias to our tmux_esp.sh script
alias vesp="~/code/one-hour-workshop/tmux_esp.sh"

# to edit an espidf project, run
% vesp <path-to-project>
```

## Nvim key mappings

Note: Leader is set to space by default in this config.

| **Shortcut** | **Mode** | **Action** |
| --- | --- | --- |
| `<leader>pv` | Normal | Open **netrw** file explorer. |
| `<leader>ff` | Normal | **Telescope**: Find files. |
| `<leader>fg` | Normal | **Telescope**: Live grep search. |
| `<leader>fb` | Normal | **Telescope**: List open buffers. |
| `<leader>q` | Normal | Quit current buffer. |
| `<leader>w` | Normal | Save current file. |
| **Git Commands (Fugitive)** |     |     |
| `<leader>gs` | Normal | Open **Git status** (Fugitive). |
| `<leader>ga` | Normal | **Git add** (Stage current file). |
| `<leader>gc` | Normal | **Git commit** (Opens commit message). |
| `<leader>gp` | Normal | **Git push** to remote. |
| `<leader>gd` | Normal | **Git diff** (Split diff view). |
| `<leader>gr` | Normal | **Git reset file** (Revert file changes). |
| **Clipboard & Editing** |     |     |
| `<leader>p` | Visual | Paste while **preserving clipboard**. |
| `<leader>y` | Normal/Visual | Copy to system clipboard. |
| `<leader>Y` | Normal | Copy entire line to clipboard. |
| `<leader>d` | Normal/Visual | **Delete** without yanking. |
| **Code Formatting & Indentation** |     |     |
| `<leader>f` | Normal | **Format code** using LSP. |
| `<` | Visual | Indent left and stay in **Visual Mode**. |
| `>` | Visual | Indent right and stay in **Visual Mode**. |
| **Auto Format on Save** |     |     |
| Auto | Normal | **Formats file on save** using LSP. |

## nvim + fugitive

### ** Fugitive Git Status Window Shortcuts**

| **Shortcut** | **Action** |
| --- | --- |
| `-` | Stage/Unstage file under cursor (**toggle add/remove**). |
| `cc` | Commit staged files (**opens commit message editor**). |
| `ca` | Amend last commit (**edit message + add new changes**). |
| `P` | Push to remote (`git push`). |
| `u` | Unstage a file (same as `git reset HEAD <file>`). |
| `D` | View **diff** of file under cursor. |
| `<Tab>` | Open file under cursor **without staging**. |
| `X` | Discard changes in file (**reset to HEAD**). |
| `q` | Quit Git status window. |

## tmux key mappings

Note: Tmux leader is set to ctrl+b by default.

#### **Session & Pane Management**

| Keybinding | Description |
| --- | --- |
| `vesp <project-folder>` | Launches a new tmux session named `esp`, opens Neovim on the left pane, and sets up the ESP-IDF environment in the right pane. |
| `tmux kill-session -t esp` | Terminates the tmux session manually. |
| `<Prefix> Q` | Terminates the tmux session. |

#### **Pane Navigation & Resizing**

| Keybinding | Description |
| --- | --- |
| `<Prefix> [` | Shrink the left (Neovim) pane by 5 columns. |
| `<Prefix> ]` | Expand the left (Neovim) pane by 5 columns. |
| `<Prefix> \` | Swap the editor and terminal panes. |

#### **ESP-IDF Development Commands**

| Keybinding | Description |
| --- | --- |
| `<Prefix> B` | Runs `idf.py build` to compile the ESP-IDF project. |
| `<Prefix> F` | Runs `idf.py flash monitor` to flash the firmware and start monitoring serial output. |
| `<Prefix> X` | Stops running serial monitoring command. |

#### **Git Commands**

| Keybinding | Description |
| --- | --- |
| `<Prefix> G` | Runs `git status` to check repository state. |
| `<Prefix> A` | Runs `git add --patch` to interactively stage changes. |
| `<Prefix> C` | Runs `git commit` to commit staged changes. |
| `<Prefix> P` | Runs `git push` to push commits to the remote repository. |

## License

MIT
