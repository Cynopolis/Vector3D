import numpy as np

def householder_reflection(A):
    """
    Perform QR decomposition using Householder reflection.
    
    Arguments:
    A -- A matrix to be decomposed (m x n).
    
    Returns:
    Q -- Orthogonal matrix (m x m).
    R -- Upper triangular matrix (m x n).
    """
    A = A.astype(float)  # Ensure the matrix is of type float
    m, n = A.shape
    Q = np.eye(m)  # Initialize Q as an identity matrix
    R = A.copy()  # R starts as a copy of A

    # Apply Householder reflections for each column
    for k in range(n):
        # Step 1: Compute the Householder vector
        x = R[k:m, k]
        e1 = np.zeros_like(x)
        e1[0] = np.linalg.norm(x) if x[0] >= 0 else -np.linalg.norm(x)
        v = x + e1
        v = v / np.linalg.norm(v)  # Normalize v
        
        # Step 2: Apply the reflection to the matrix
        R[k:m, k:n] = R[k:m, k:n] - 2 * np.outer(v, v.T @ R[k:m, k:n])
        
        # Step 3: Apply the reflection to Q
        Q[:, k:m] = Q[:, k:m] - 2 * np.outer(Q[:, k:m] @ v, v)

    # The resulting Q and R are the QR decomposition
    return Q, R

# Example usage
A = np.array([[12, -51, 4], 
              [6, 167, -68], 
              [-4, 24, -41]])

Q, R = householder_reflection(A)
print("Q matrix:")
print(Q)
print("\nR matrix:")
print(R)
print("Multiplied Together:")
print(Q@R)

def svd_decomposition(A):
    """
    Perform Singular Value Decomposition (SVD) from scratch.
    
    Arguments:
    A -- The matrix to be decomposed (m x n).
    
    Returns:
    U -- Orthogonal matrix of left singular vectors (m x m).
    Sigma -- Diagonal matrix of singular values (m x n).
    Vt -- Orthogonal matrix of right singular vectors (n x n).
    """
    # Step 1: Compute A^T A
    AtA = np.dot(A.T, A)  # A transpose multiplied by A
    
    # Step 2: Compute the eigenvalues and eigenvectors of A^T A
    eigenvalues, V = np.linalg.eig(AtA)
    
    # Step 3: Sort eigenvalues in descending order and sort V accordingly
    sorted_indices = np.argsort(eigenvalues)[::-1]  # Indices to sort eigenvalues in descending order
    eigenvalues = eigenvalues[sorted_indices]
    V = V[:, sorted_indices]
    
    # Step 4: Compute the singular values (sqrt of eigenvalues)
    singular_values = np.sqrt(eigenvalues)
    
    # Step 5: Construct the Sigma matrix
    m, n = A.shape
    Sigma = np.zeros((m, n))  # Initialize Sigma as a zero matrix
    for i in range(min(m, n)):
        Sigma[i, i] = singular_values[i]  # Place the singular values on the diagonal
    
    # Step 6: Compute the U matrix using A * V = U * Sigma
    U = np.dot(A, V)  # A * V gives us the unnormalized U
    # Normalize the columns of U
    for i in range(U.shape[1]):
        U[:, i] = U[:, i] / singular_values[i]  # Normalize each column by the corresponding singular value
    
    # Step 7: Return U, Sigma, Vt
    return U, Sigma, V.T  # V.T is the transpose of V

# Example usage
A = np.array([[12, -51, 4],
              [6, 167, -68],
              [-4, 24, -41]])

U, Sigma, Vt = svd_decomposition(A)

print("\nSVD DECOMPOSITION\nU matrix:")
print(U)
print("\nSigma matrix:")
print(Sigma)
print("\nVt matrix:")
print(Vt)
print("Multiplied together:")
print(U@Sigma@Vt)
