# 📇 Contact Manager CLI in C

[![Language: C](https://img.shields.io/badge/Language-C-00599C?logo=c&logoColor=white)](https://en.cppreference.com/w/c)
[![Interface: CLI](https://img.shields.io/badge/Interface-CLI-black?logo=gnuterminal&logoColor=white)](https://en.wikipedia.org/wiki/Command-line_interface)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A command-line contact manager written in C. It supports adding, listing, searching, editing, and deleting contacts with file-based persistence.

## ✨ Highlights

- Add, list, search, edit, and delete contacts
- File-backed storage in `All-Contact.txt`
- Case-insensitive name matching for search, edit, and delete
- Duplicate name handling with numbered selection
- Phone and email uniqueness validation

## 📥 Clone

```text
git clone https://github.com/litch07/contact-manager-c.git
cd contact-manager-c
```

## 🛠️ Build (Windows)

```text
gcc main.c -o contact-manager.exe
```

## ▶️ Run (Windows)

```text
contact-manager.exe
```

## 🎯 Usage Notes

- Each contact is stored as three lines: name, phone, email.
- Maximum lengths: name 50, phone 25, email 254 characters.
- If multiple contacts share a name, you can choose which one to edit or delete.

## 🧾 Example (CLI)

```text
Contact Management System
==========================
Main Menu
1. Add a new contact
2. Show all contacts
3. Search for a contact
4. Edit a contact
5. Delete a contact
6. Delete all contacts
i. User guide
a. About
0. Exit
--------------------------
Enter your choice:
```

## 📁 Files

- `main.c` - application source
- `All-Contact.txt` - contact storage (auto-created)
- `README.md` - documentation

## 📄 License

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.