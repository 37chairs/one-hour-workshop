require("espdev.set")
require("espdev.remap")

local augroup = vim.api.nvim_create_augroup
local espDevGroup = augroup('espdev', {})

local autocmd = vim.api.nvim_create_autocmd

-- Bootstrap packer if not installed
local ensure_packer = function()
  local fn = vim.fn
  local install_path = fn.stdpath('data') .. '/site/pack/packer/start/packer.nvim'
  if fn.empty(fn.glob(install_path)) > 0 then
    fn.system({ 'git', 'clone', '--depth', '1', 'https://github.com/wbthomason/packer.nvim', install_path })
    vim.cmd [[packadd packer.nvim]]
    return true
  end
  return false
end

local packer_bootstrap = ensure_packer()

function read_api_key()
  local home = os.getenv("HOME") or "~"
  local file_path = home .. "/.local/openai_api_key.txt"

  -- Ensure the file exists before returning the command
  local file = io.open(file_path, "r")
  if file then
    file:close()
    return "cat " .. file_path -- Return a shell command, not the key itself
  else
    print("Error: OpenAI API key file not found at " .. file_path)
    return nil
  end
end

return require('packer').startup(function(use)
  use 'wbthomason/packer.nvim' -- Packer manages itself

  -- Plugins
  use 'folke/zen-mode.nvim'
  use 'github/copilot.vim'
  use 'tpope/vim-fugitive' -- Git integration

  -- only load openai if ENV var OPENAI_API_KEY is set.
  if os.getenv("OPENAI_API_KEY") then
    use {
      "jackMort/ChatGPT.nvim",
      requires = {
        "MunifTanjim/nui.nvim",
        "nvim-lua/plenary.nvim",
        "nvim-telescope/telescope.nvim"
      },
      config = function()
        require("chatgpt").setup({})
      end
    }
  end

  use {
    'nvim-lualine/lualine.nvim',
    requires = { 'nvim-tree/nvim-web-devicons', opt = true },
    config = function()
      require('lualine').setup({
        options = {
          theme = 'auto',
          globalstatus = true,
          section_separators = '',
          component_separators = '',
        },
        sections = {
          lualine_a = { 'mode' },
          lualine_b = {
            'branch',
            'diff',
            {
              'diagnostics',
              sources = { 'nvim_lsp' },
              sections = { 'error', 'warn', 'info', 'hint' },
              symbols = { error = ' ', warn = ' ', info = ' ', hint = ' ' }
            }
          },
          lualine_c = {
            'filename',
            {
              function()
                local clients = vim.lsp.get_active_clients()
                if next(clients) == nil then
                  return 'No LSP'
                end
                local names = {}
                for _, client in ipairs(clients) do
                  table.insert(names, client.name)
                end
                return 'LSP: ' .. table.concat(names, ', ')
              end,
              icon = '',
            }
          },
          lualine_x = { 'encoding', 'fileformat', 'filetype' },
          lualine_y = { 'progress' },
          lualine_z = { 'location' },
        }
      })
    end
  }

  -- Treesitter for syntax highlighting
  use {
    'nvim-treesitter/nvim-treesitter',
    run = ':TSUpdate',
    config = function()
      require('nvim-treesitter.configs').setup({
        ensure_installed = { "c", "cpp", "lua", "python", "json", "yaml", "bash", "make", "cmake", "javascript",
          "typescript" },
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
    requires = { { 'nvim-lua/plenary.nvim' } }
  }

  -- Harpoon (Quick navigation)
  use {
    'ThePrimeagen/harpoon',
    requires = { 'nvim-lua/plenary.nvim' }
  }

  -- LSP and completion setup
  use {
    "williamboman/mason.nvim",
    config = function()
      require("mason").setup()
    end
  }

  use {
    "neovim/nvim-lspconfig",
    after = { "mason.nvim" },
    config = function()
      local lspconfig = require("lspconfig")
      local capabilities = require("cmp_nvim_lsp").default_capabilities()

      -- Only setup clangd and lua_ls here; ESLint is handled in mason-lspconfig
      lspconfig.clangd.setup {
        capabilities = capabilities,
        cmd = { "/usr/bin/clangd" }
      }
      lspconfig.lua_ls.setup {
        capabilities = capabilities
      }
    end
  }

  use {
    "williamboman/mason-lspconfig.nvim",
    after = "nvim-lspconfig", -- Ensure lspconfig loads first
    requires = { "williamboman/mason.nvim", "neovim/nvim-lspconfig" },
    config = function()
      local lspconfig = require("lspconfig")
      local capabilities = require("cmp_nvim_lsp").default_capabilities()

      require("mason-lspconfig").setup({
        ensure_installed = { "lua_ls", "eslint" },
        handlers = {
          function(server_name)
            lspconfig[server_name].setup({
              capabilities = capabilities
            })
          end,
          ["eslint"] = function()
            lspconfig.eslint.setup({
              capabilities = capabilities,
              root_dir = require("lspconfig.util").root_pattern(".eslintrc", "package.json"),
              settings = {
                validate = "on",
                format = {
                  enable = true
                },
                lintTask = {
                  enable = true
                },
                packageManager = "npm"
              },
              on_attach = function(client, bufnr)
                -- Enable formatting from ESLint
                client.server_capabilities.documentFormattingProvider = true

                -- Run `EslintFixAll` before saving
                vim.api.nvim_create_autocmd("BufWritePre", {
                  group = espDevGroup,
                  buffer = bufnr,
                  command = "EslintFixAll"
                })
              end
            })
          end
        }
      })
    end
  }

  use {
    'hrsh7th/nvim-cmp',
    requires = { 'hrsh7th/cmp-nvim-lsp', 'hrsh7th/cmp-buffer', 'hrsh7th/cmp-path', 'hrsh7th/cmp-cmdline',
      'hrsh7th/cmp-nvim-lua', 'L3MON4D3/LuaSnip' },
    config = function()
      local cmp = require('cmp')
      cmp.setup({
        mapping = {
          ['<C-Space>'] = cmp.mapping.complete(),
          ['<CR>'] = cmp.mapping.confirm({
            select = true
          })
        },
        sources = { {
          name = 'nvim_lsp'
        }, {
          name = 'buffer'
        }, {
          name = 'path'
        } }
      })
    end
  }

  if packer_bootstrap then
    require('packer').sync()
  end
end)
