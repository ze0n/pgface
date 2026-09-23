# pgface

Terminal interface for browsing and editing a PostgreSQL database.
It is an ncurses application built on CDK and libpq: a menu bar, schema and table lists, a data grid, and a small SQL editor.

## Requirements

- a C++ compiler (the Makefile uses g++)
- libpq
- ncurses (linked as libcurses)
- CDK (libcdk) — <http://invisible-island.net/cdk/>

## Building

```sh
make
```

This produces the `pgface` binary in the current directory.

```sh
make clean
```

removes the binary and object files.

Run pgface from a directory that contains `pgface.conf`. The program always opens `./pgface.conf`, regardless of where the binary itself lives.

## Configuration

`pgface.conf` is required. If it is missing, or if neither `host` nor `hostaddr` is set, pgface reports an error and exits.

Each setting is one line in the form `option = value`: a single space on each side of `=`, no tabs. A line whose first character is `#` is a comment. Unrecognized lines are ignored. Each value line must end with a newline; the parser drops the last character of the line.

Recognized options:

```ini
hostaddr = 127.0.0.1
host = localhost
port = 5432
dbname = mydb
user = myuser
password = secret
```

`hostaddr` is used when it is set; otherwise `host` is used. One of them must be present. `port` defaults to the libpq default (5432) when omitted.

File → Options shows the connection string built from this file. The file is read only at startup.

## Running

```text
pgface              start the interface
pgface -v           also show each INSERT, UPDATE, and DELETE before it runs
pgface -h           short description (also --help or -?)
```

On startup pgface loads the config and asks whether to connect. After a successful connection it lists schemas.

## Screen

A path line sits under the menu and a status line sits at the bottom.

Menu (open and close with F1 or Esc; move with the arrow keys, choose with Enter):

| Menu | Item | Action |
| --- | --- | --- |
| File | Options... | Show the connection string |
| File | Exit | Leave, after confirmation |
| Tools | Connect | Connect and open the schema list |
| Tools | Disconnect | Drop the connection and return to the database list |
| Tools | Qery tool | SQL editor (only while connected, and not while a record form is open) |
| Help | Help | One-line reminder of the data-grid keys |

Browsing:

- **Databases** — shown while disconnected; the only entry is the database named in the config. Enter opens schemas and prompts to connect.
- **Schemas** — name and owner, from `pg_namespace`. Enter selects a schema (`search_path` is set to it) and lists its tables. `..` disconnects, after confirmation.
- **Tables** — ordinary tables only (`relkind = 'r'`), with owner. System schemas are included. Enter opens the table with `SELECT *`. `..` returns to the schema list.

The data grid is that `SELECT *`. Arrow keys move between cells.

## Keys

| Context | Key | Action |
| --- | --- | --- |
| Menu | F1 or Esc | Open or close |
| Menu | arrows | Move |
| Menu | Enter | Choose |
| Schema and table lists | arrows | Move |
| Schema and table lists | Enter | Choose |
| Schema and table lists | F1 or Esc | Menu |
| Data grid | arrows | Move |
| Data grid | e or F4 | Edit the current row |
| Data grid | a or i | Append a row |
| Data grid | d | Delete the current row (asks first) |
| Data grid | x or Esc | Close the grid |
| Record form | Tab or Ctrl+N | Next column |
| Record form | Ctrl+P | Previous column |
| Record form | Enter | Save (also the Confirm button; Space activates a focused button) |
| Record form | Esc | Cancel |
| SQL editor | Enter | Run the statement (also the Perform button) |
| SQL editor | Close | Leave the editor |

## Editing rows

Append and edit open one field per column, then Confirm and Cancel.
A blank field is written as NULL. Filled fields are written as quoted text.

Update and delete find the row by comparing every column with the values that were on screen. There is no use of a primary key. A cell that was empty is not compared. Because of that, two identical rows cannot be told apart, and a delete or update can affect more than the row you highlighted.

## SQL editor

Tools → Qery tool opens a multi-line entry labeled `SQL:`.

A statement whose text begins with `select` (any case) is run as a query and the rows are shown in a grid. Any other statement is run as a command; pgface then reports success or failure and does not show a result grid.

Both kinds of statement run inside a transaction that is committed on success and rolled back on failure.
