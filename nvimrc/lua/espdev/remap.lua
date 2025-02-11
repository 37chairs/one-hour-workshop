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

vim.keymap.set("v", "<", "<gv", { desc = "Indent Left" })
vim.keymap.set("v", ">", ">gv", { desc = "Indent Right" })

vim.api.nvim_create_autocmd("BufWritePre", {
    pattern = "*",
    callback = function()
        vim.lsp.buf.format({ async = false })
    end,
})


