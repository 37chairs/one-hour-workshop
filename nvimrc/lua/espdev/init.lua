require("espdev.set")
require("espdev.remap")

local augroup = vim.api.nvim_create_augroup
local espdevGroup = augroup('espdev', {})

local autocmd = vim.api.nvim_create_autocmd

-- Bootstrap packer if not installed
local ensure_packer = function()
    local fn = vim.fn
    local install_path = fn.stdpath('data') .. '/site/pack/packer/start/packer.nvim'
    if fn.empty(fn.glob(install_path)) > 0 then
        fn.system({'git', 'clone', '--depth', '1', 'https://github.com/wbthomason/packer.nvim', install_path})
        vim.cmd [[packadd packer.nvim]]
        return true
    end
    return false
end

local packer_bootstrap = ensure_packer()

return require('packer').startup(function(use)
    use 'wbthomason/packer.nvim' -- Packer manages itself

    -- Plugins
    use 'folke/zen-mode.nvim'
    use 'github/copilot.vim'
    use 'tpope/vim-fugitive' -- Git integration

    -- Treesitter for syntax highlighting and ESP-IDF support
    use {
        'nvim-treesitter/nvim-treesitter',
        run = ':TSUpdate',
        config = function()
            require('nvim-treesitter.configs').setup({
                ensure_installed = {"c", "cpp", "lua", "python", "json", "yaml", "bash", "make", "cmake"},
                highlight = {
                    enable = true
                },
                indent = {
                    enable = true
                }
            })
        end
    }
    use("nvim-treesitter/playground")

    -- Telescope (Fuzzy finder)
    use {
        'nvim-telescope/telescope.nvim',
        tag = '0.1.2',
        requires = {{'nvim-lua/plenary.nvim'}}
    }

    -- Harpoon (Quick navigation)
    use {
        'ThePrimeagen/harpoon',
        requires = {'nvim-lua/plenary.nvim'}
    }

    -- LSP Config (clangd for C/C++)
    use {
        'neovim/nvim-lspconfig',
        config = function()
            require('lspconfig').clangd.setup({})
        end
    }

    use {
    "williamboman/mason.nvim",
    config = function()
        require("mason").setup()
    end
}

use {
    "williamboman/mason-lspconfig.nvim",
    after = "mason.nvim",
    requires = { "neovim/nvim-lspconfig" },
    config = function()
        require("mason-lspconfig").setup({
            ensure_installed = { "clangd", "lua_ls" } -- Auto-install servers
        })
    end
}

use {
    "neovim/nvim-lspconfig",
    after = "mason-lspconfig",
    config = function()
        local lspconfig = require("lspconfig")

        -- C/C++ Language Server
        lspconfig.clangd.setup{}

        -- Lua Language Server
        lspconfig.lua_ls.setup{}
    end
}

use {
    "hrsh7th/nvim-cmp", -- Completion engine
    requires = {
        "hrsh7th/cmp-nvim-lsp", -- LSP completion source
        "L3MON4D3/LuaSnip" -- Snippet engine
    },
    config = function()
        local cmp = require("cmp")
        cmp.setup({
            mapping = cmp.mapping.preset.insert({
                ["<C-Space>"] = cmp.mapping.complete(),
                ["<CR>"] = cmp.mapping.confirm({ select = true }),
            }),
            sources = cmp.config.sources({
                { name = "nvim_lsp" },
            })
        })
    end
}


    -- Autocompletion
    use {
        'hrsh7th/nvim-cmp',
        requires = {'hrsh7th/cmp-nvim-lsp', 'hrsh7th/cmp-buffer', 'hrsh7th/cmp-path', 'hrsh7th/cmp-cmdline',
                    'hrsh7th/cmp-nvim-lua'},
        config = function()
            local cmp = require('cmp')
            cmp.setup({
                mapping = {
                    ['<C-Space>'] = cmp.mapping.complete(),
                    ['<CR>'] = cmp.mapping.confirm({
                        select = true
                    })
                },
                sources = {{
                    name = 'nvim_lsp'
                }, {
                    name = 'buffer'
                }, {
                    name = 'path'
                }}
            })
        end
    }

    if packer_bootstrap then
        require('packer').sync()
    end
end)

