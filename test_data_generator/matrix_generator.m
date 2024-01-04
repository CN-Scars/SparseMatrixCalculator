% Matrix Generator Script
% Description: This script generates various kinds of matrices including 
% zero matrices, dense matrices, sparse matrices, and their large-scale versions.
% 
% Author: [CN-Scars]
% Created: [2024.1.4]
% 
% License: [MIT License]
% 
% Copyright (c) [2024] [Scars (Guoyu Su)]
%
% Permission is hereby granted, free of charge, to any person obtaining a copy
% of this software and associated documentation files (the "Software"), to deal
% in the Software without restriction, including without limitation the rights
% to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
% copies of the Software, and to permit persons to whom the Software is
% furnished to do so, subject to the following conditions:
%
% The above copyright notice and this permission notice shall be included in all
% copies or substantial portions of the Software.
%
% THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
% IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
% FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
% AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
% LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
% OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
% SOFTWARE.

function matrix_generator()
    % 创建文件夹
    mkdir('zero_matrices');
    mkdir('dense_matrices');
    mkdir('sparse_matrices');
    mkdir('large_dense_matrices');
    mkdir('large_sparse_matrices');
    
    % 生成测试数据
    generate_zero_matrices();
    generate_dense_matrices(10, 10); % 例如 10x10 的稠密矩阵
    generate_sparse_matrices(10, 10, 0.1); % 例如 10x10 的稀疏矩阵，稀疏度为0.1
    generate_dense_matrices(1000, 1000); % 超大的稠密矩阵
    generate_sparse_matrices(1000, 1000, 0.001); % 超大的稀疏矩阵
end

function generate_zero_matrices()
    A = zeros(10, 10); % 例如 10x10 的0矩阵
    B = zeros(10, 10);
    save_full_matrix_to_txt(A, 'zero_matrices/matrixA.txt');
    save_full_matrix_to_txt(B, 'zero_matrices/matrixB.txt');
end

function generate_dense_matrices(rows, cols)
    A = rand(rows, cols);
    B = rand(rows, cols);
    save_full_matrix_to_txt(A, 'dense_matrices/matrixA.txt');
    save_full_matrix_to_txt(B, 'dense_matrices/matrixB.txt');
end

function generate_sparse_matrices(rows, cols, density)
    A = sprand(rows, cols, density);
    B = sprand(rows, cols, density);
    save_full_matrix_to_txt(A, 'sparse_matrices/matrixA.txt');
    save_full_matrix_to_txt(B, 'sparse_matrices/matrixB.txt');
end

function save_full_matrix_to_txt(matrix, filename)
    fileID = fopen(filename, 'w');
    [m, n] = size(matrix);
    for i = 1:m
        for j = 1:n
            fprintf(fileID, '%f ', full(matrix(i, j)));
        end
        fprintf(fileID, '\n');
    end
    fclose(fileID);
end

