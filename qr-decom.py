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
