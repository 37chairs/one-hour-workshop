-- Set leader to space
vim.g.mapleader = " " 
vim.g.maplocalleader = " "

-- Keymaps for Telescope
vim.keymap.set("n", "<leader>pv", vim.cmd.Ex) -- Open netrw file explorer

vim.keymap.set("n", "<leader>ff", "<cmd>Telescope find_files<cr>", { desc = "Find Files" })
vim.keymap.set("n", "<leader>fg", "<cmd>Telescope live_grep<cr>", { desc = "Live Grep" })
vim.keymap.set("n", "<leader>fb", "<cmd>Telescope buffers<cr>", { desc = "List Buffers" })

vim.keymap.set("n", "<leader>q", "<cmd>q<cr>") -- Quickly quit a buffer
vim.keymap.set("n", "<leader>w", "<cmd>w<cr>") -- Quickly save a file

-- Open Fugitive Git Status
vim.keymap.set("n", "<leader>gs", vim.cmd.Git, { desc = "Git Status (Fugitive)" })

-- Git Add and Commit
vim.keymap.set("n", "<leader>gap", ":Git add -p<CR>", { desc = "Git Add Patch Mode" })
vim.keymap.set("n", "<leader>ga", ":Gwrite<CR>", { desc = "Git Add (Stage Current File)" })
vim.keymap.set("n", "<leader>gc", ":Git commit<CR>", { desc = "Git Commit" })
vim.keymap.set("n", "<leader>gp", ":Git push<CR>", { desc = "Git Push" })

-- Git Diff
vim.keymap.set("n", "<leader>gd", ":Gdiffsplit<CR>", { desc = "Git Diff" })

-- Git Reset Current File
vim.keymap.set("n", "<leader>gr", ":Gread<CR>", { desc = "Git Reset (Revert File)" })


-- greatest remap ever
vim.keymap.set("x", "<leader>p", [["_dP]])

-- next greatest remap ever : asbjornHaland
vim.keymap.set({"n", "v"}, "<leader>y", [["+y]])
vim.keymap.set("n", "<leader>Y", [["+Y]])

vim.keymap.set({"n", "v"}, "<leader>d", [["_d]])

vim.keymap.set("n", "<leader>f", function()
    vim.lsp.buf.format({ async = true })
end, { desc = "Format Code (LSP)" })

-- LSP Navigation
vim.keymap.set('n', 'gd', vim.lsp.buf.definition, { desc = "Go to Definition" })
vim.keymap.set('n', 'gr', vim.lsp.buf.references, { desc = "Go to References" })
vim.keymap.set('n', 'gi', vim.lsp.buf.implementation, { desc = "Go to Implementation" })
vim.keymap.set('n', 'K', vim.lsp.buf.hover, { desc = "Show Hover Documentation" })
vim.keymap.set('n', '<leader>rn', vim.lsp.buf.rename, { desc = "Rename Symbol" })
vim.keymap.set('n', '<leader>ca', vim.lsp.buf.code_action, { desc = "Code Actions" })


vim.keymap.set("v", "<", "<gv", { desc = "Indent Left" })
vim.keymap.set("v", ">", ">gv", { desc = "Indent Right" })

vim.api.nvim_create_autocmd("BufWritePre", {
    pattern = "*",
    callback = function()
        vim.lsp.buf.format({ async = false })
    end,
})

-- ChatGPT.nvim key mappings
vim.api.nvim_set_keymap("v", "<leader>ai", ":ChatGPTEditWithInstructions<CR>", { noremap = true, silent = true })
vim.api.nvim_set_keymap("v", "<leader>ae", ":ChatGPTRun explain_code<CR>", { noremap = true, silent = true })
vim.api.nvim_set_keymap("v", "<leader>at", ":ChatGPTRun translate_code<CR>", { noremap = true, silent = true })
vim.api.nvim_set_keymap("v", "<leader>ad", ":ChatGPTRun docstring<CR>", { noremap = true, silent = true })
vim.api.nvim_set_keymap("n", "<leader>ac", ":ChatGPT<CR>", { noremap = true, silent = true })
vim.api.nvim_set_keymap("n", "<leader>ah", ":ChatGPTRun fix_bugs<CR>", { noremap = true, silent = true })
vim.api.nvim_set_keymap("n", "<leader>ag", ":ChatGPTRun grammar_correction<CR>", { noremap = true, silent = true })

