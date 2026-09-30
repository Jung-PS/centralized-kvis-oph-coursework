# Centralized IAM System for KVIS Open House
* A purpose concept of Centralized IAM System for KVIS Open House
* A capstone project for Computer Programming 2, AY2026, Kamnoetvidya Science Academy
<img width="512" alt="image" src="https://github.com/user-attachments/assets/888a0956-ef8f-41e0-aa6d-ab9af2c96831" />

---

This is free software, it is hereby released under the MIT License

Designed to be compatible with GNU/Linux Operating system
  * Compiled on g++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0
  * Running on Ubuntu 24.04.4 LTS (Noble Numbat)
  * Within the distrobox podman container of Fedora Linux 44.20260924.0 (Silverblue)

```cpp
// FOR CLEAR
char cache[1024];
new_user(user_data, &user_config_data, "ADMIN", 123456,cache);
change_user_balance(user_data, user_config_data, 1, 123456, 10000000);
change_user_role(user_data, user_config_data, 1, "01100000000000000000");
//
```
Remove this Chunk to allow database persistence

