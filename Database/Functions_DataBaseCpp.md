**WorkFlow of Get or Create fileid(path of a file) function**

**GetOrCreateFileId(path)**

        │
        ▼

SELECT file_id
FROM files
WHERE path=?

        │
        ├─────────────┐
        │             │
        ▼             ▼

Found (fast)         Not Found(Slow, Time consuming Process)

return id      INSERT row

                   │
                   ▼

      sqlite3_last_insert_rowid()

                   │
                   ▼

              return new id