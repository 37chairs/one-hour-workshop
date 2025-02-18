# One Hour Workshop

neovim setup and config for espidf development.  I borrowed some of the lua structure and defaults from [ThePrimeagen](https://github.com/ThePrimeagen).


## editor requirements

- install neovim 0.7+ -- make sure your version of neovim has LuaJIT support.  I noticed neovim on Ubuntu 22.04 does not, but 24.04 does.

```bash
nvim --version
NVIM v0.9.5
Build type: Release
LuaJIT 2.1.1703358377
```

```bash

mkdir -p ~/.local/bin
mv nvim-linux-arm64.appimage ~/.local/bin/nvim
chmod +x ~/.local/bin/nvim
echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc
```

## Using ChatGPT

To use ChatGPT, generate an API key and save it in ~/.local/openai_api_key.txt.

Then in .bashrc: 
```
export OPENAI_API_KEY=$(cat ~/.local/openai_api_key.txt)
```

## nvimrc

TODO: add note re: installing  vim packages and language servers
Another test change

```bash
# edit ~/.zshrc and add an alias to our nvim + esp config
alias vesp="source ~/.local/.espdev_env && nvim -u ~/code/one-hour-workshop/nvimrc/init.lua"


# to edit an espidf project, run
% cd <path-to-project>
% vesp .
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

#### **ESP-IDF Development Commands**

| Keybinding | Description |
| --- | --- |
| `<leader>eb` | Runs `idf.py build` to compile the ESP-IDF project. |
| `<leader>ef` | Runs `idf.py flash monitor` to flash the firmware and start monitoring serial output. |
| `<leader>em` | Stops running serial monitoring command. |

## License

MIT
