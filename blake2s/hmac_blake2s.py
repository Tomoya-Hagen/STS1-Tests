from cryptography.hazmat.primitives import hashes, hmac
from cryptography.hazmat.backends import default_backend

def hmac_blake2s(key, data):
    # Initialize HMAC with BLAKE2s
    h = hmac.HMAC(key, hashes.BLAKE2s(32), backend=default_backend())
    h.update(data)
    # Finalize and return the full hash
    full_hash = h.finalize()
    # Truncate the hash to 64 bits (8 bytes)
    truncated_hash = full_hash[:8]
    return truncated_hash

if __name__ == "__main__":
    # for: testBasicSignatureGeneration()
    # key = b"testkey123" # 32-byte key
    # data = b"Uplink data example message"  # 64-byte data
    # for: hash_empty_input() (the key also for single_block...)
    key = b'\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F' \
          b'\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F'   # 32-byte key
    # data = b""
    # for: test_hmac()
    # key = bytes([5, 3, 1, 2, 6, 4, 8, 2, 10, 19, 2, 7, 2, 5, 3, 1])
    # data = bytes([2, 5, 8, 12, 9, 10, 45])
    # for: single_block_input()
    # data = b"This frog has been stripped of the owner. It is now too hot too"
    # for: hash_double_block_input()
    # data = b"012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012"
    # key = b"oida!"
    # for: hash_partial_block_input()
    # data = bytes([ord(c) for c in 'Exam'])
    # key = bytes([ord(c) for c in 'ae&#3GK!)]'])
    data = bytes([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
                         42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
                         42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
                         42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
                         42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64])
    key = bytes([1, 98, 99, 90, 9, 89, 77, 200, 199, 99, 9, 159])
    hash_output = hmac_blake2s(key, data)

    # Print the resulting hash (64-bit truncated hash)
    print("HMAC-BLAKE2s-64 output:", hash_output.hex())
