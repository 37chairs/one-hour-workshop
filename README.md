# One Hour Workshop

neovim setup and config for espidf development.  I borrowed some of the lua structure and defaults from [ThePrimeagen](https://github.com/ThePrimeagen).

## editor requirements

- install tmux, neovim

## nvimrc

```bash
# edit ~/.zshrc and add an alias to our tmux_esp.sh script
alias vesp="~/code/one-hour-workshop/tmux_esp.sh"

# to edit an espidf project, run
% vesp <path-to-project>
```

## Nvim key mappings

Note: Leader is set to space by default in this config.

| **Keybinding** | **Mode** | **Action** |
| --- | --- | --- |
| `<leader>pv` | Normal | Open **netrw file explorer** |
| `<leader>ff` | Normal | Open **Telescope** to **find files** |
| `<leader>fg` | Normal | Open **Telescope** for **live grep** search |
| `<leader>fb` | Normal | Open **Telescope** to **list buffers** |
| `<leader>gs` | Normal | Open **Git status** (vim-fugitive) |
| `<leader>q` | Normal | Quickly **quit** a buffer (`:q`) |
| `<leader>w` | Normal | Quickly **save** a file (`:w`) |
| `<leader>p` | Visual | Replace selection with **pasted** content (without overwriting register) |
| `<leader>y` | Normal/Visual | Copy to **system clipboard** (`"+y`) |
| `<leader>Y` | Normal | Copy **entire line** to **system clipboard** (`"+Y`) |
| `<leader>d` | Normal/Visual | Delete without modifying register (`"_d`) |
| `<leader>f` | Normal | **Format code** using **LSP formatter** |
| `<leader>i` | Normal | **Auto-indent entire file** (`gg=G`) |
| `<` / `>` | Visual | **Indent selection** left/right |
| **Auto-Format on Save** | Auto | **LSP formatting** runs every time you save (`:w`) |

## tmux key mappings

Note: Tmux leader is set to ctrl+b by default.

### **Tmux Keybindings**

| **Keybinding** | **Action** |
| --- | --- |
| **`[`** | Shrink **Neovim pane** (move divider **left** by 5 columns). |
| **`]`** | Expand **Neovim pane** (move divider **right** by 5 columns). |
| **`\`** | Swap **left and right** panes. |
| **`B`** | Run `idf.py build` in the **ESP-IDF terminal**. |
| **`F`** | Run `idf.py flash monitor` in the **ESP-IDF terminal**. |
| **`Q`** | Kill the **tmux session** (`tmux kill-session -t esp`). |

## tmux

1. Ctrl-b, B will run the build command
2. Ctrl-b, F will run the flash command


## license

MIT
