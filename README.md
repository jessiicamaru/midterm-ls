# Cài đặt lại lệnh `ls(1)` trong UNIX

**Họ và tên:** Hoàng Công Dũng

**Mã sinh viên:** 23IT036

---

## 1. Giới thiệu

Mục tiêu của bài tập giữa kỳ này là **tự xây dựng lại lệnh `ls` trong hệ điều hành Linux** – một trong những lệnh cơ bản nhưng quan trọng trong việc quản lý file và thư mục.

Dự án được phát triển hoàn toàn bằng ngôn ngữ **C**, tuân thủ chuẩn **POSIX**, mô phỏng theo tài liệu hướng dẫn (manpage) của `ls` trong **NetBSD**, và có hỗ trợ nhiều tùy chọn nâng cao.

---

## 2. Mục tiêu của project

- Hiểu rõ hoạt động của lệnh `ls` trên hệ thống UNIX/Linux.
- Thực hành sử dụng các **thư viện hệ thống** như `<dirent.h>`, `<sys/stat.h>`, `<pwd.h>`, `<grp.h>`.
- Triển khai **xử lý tham số dòng lệnh** và **tùy chọn (flag)** tương tự `ls` thật.
- Rèn luyện kỹ năng **phân chia chương trình thành nhiều module** và **viết code có tổ chức**.

---

## 3. Mức độ hoàn thiện

Phiên bản cuối cùng đã **cài đặt đầy đủ các flag cơ bản và nâng cao**, bao gồm:

| Nhóm             | Flag                               | Mô tả                                                                     |
| ---------------- | ---------------------------------- | ------------------------------------------------------------------------- |
| Hiển thị cơ bản  | `-a`, `-A`, `-F`, `-i`, `-s`       | Ẩn/hiện file ẩn, hiển thị inode, block, thêm ký hiệu phân loại            |
| Liệt kê chi tiết | `-l`, `-n`, `-h`                   | In dạng dài, dùng tên hoặc UID/GID số, hiển thị kích thước dễ đọc         |
| Sắp xếp          | `-t`, `-S`, `-r`, `-u`, `-c`, `-f` | Sắp xếp theo thời gian, kích thước, đảo ngược, atime/ctime, không sắp xếp |
| Thư mục          | `-R`, `-d`                         | Liệt kê đệ quy hoặc chỉ hiển thị chính thư mục                            |
| Ký tự đặc biệt   | `-q`, `-w`                         | Xử lý ký tự không in được, chế độ raw hoặc quote                          |
| Khác             | `-k`                               | Hiển thị đơn vị kilobyte                                                  |

---

## 4. Cấu trúc thư mục dự án

```
midterm-ls/
├── src/
│   ├── main.c
│   ├── options.c
│   ├── display.c
│   ├── file_info.c
│   └── utils.c
├── include/
│   ├── ls.h
│   ├── options.h
│   ├── display.h
│   ├── file_info.h
│   └──utils.h
└── Makefile
```

---

## 5. Mô tả các module chính

### 5.1. `options.c`

Xử lý toàn bộ **phân tích tham số dòng lệnh** (`argc`, `argv`). Mỗi flag được ánh xạ thành một trường trong cấu trúc `Options`.
Các flag có tính chất **override nhau** được xử lý đúng theo quy định của manpage NetBSD:

- `-w` và `-q`: tùy chọn cuối cùng quyết định cách in ký tự không in được.
- `-l` và `-n`: tùy chọn cuối cùng quyết định hiển thị tên hay UID/GID.
- `-c` và `-u`: tùy chọn cuối cùng quyết định dùng thời gian thay đổi hay truy cập.
- `-R` và `-d`: tùy chọn cuối cùng quyết định hiển thị đệ quy hay chỉ thư mục.

```c
case 'c': opts->sort_ctime = 1; opts->sort_atime = 0; break;
case 'u': opts->sort_atime = 1; opts->sort_ctime = 0; break;
case 'R': opts->recursive = 1; opts->list_dir_as_file = 0; break;
case 'd': opts->list_dir_as_file = 1; opts->recursive = 0; break;
```

### 5.2. `display.c`

Chịu trách nhiệm **liệt kê và hiển thị thư mục hoặc tệp tin**.

- Hỗ trợ sắp xếp theo tên, thời gian, kích thước.
- Hiển thị đệ quy khi bật `-R`.
- Hiển thị inode (`-i`), block (`-s`), ký hiệu phân loại (`-F`).
- Xử lý ký tự không in được (`-q`, `-w`).

![Minh họa flag -a](https://res.cloudinary.com/dunmkypzg/image/upload/v1762114100/a_lower_gybi3p.png)

![Minh họa flag -R](https://res.cloudinary.com/dunmkypzg/image/upload/v1762112235/R_kixcfp.png)

### 5.3. `file_info.c`

In thông tin chi tiết khi bật `-l` hoặc `-n`, bao gồm:

- Quyền truy cập (`drwxr-xr-x`)
- Số liên kết (nlink)
- Chủ sở hữu (user/group hoặc UID/GID)
- Kích thước và thời gian sửa đổi gần nhất.

![Minh họa flag -l](https://res.cloudinary.com/dunmkypzg/image/upload/v1762112232/l_egl4cz.png)

![Minh họa flag -n](https://res.cloudinary.com/dunmkypzg/image/upload/v1762112234/n_pxt1aq.png)

### 5.4. `utils.c`

Chứa các hàm tiện ích dùng chung như in thông báo lỗi hoặc xử lý định dạng kích thước (`human_readable()`).

---

## 6. Build và khởi động dự án

**1.** Di chuyển vào thư mục dự án

```
git clone https://github.com/jessiicamaru/midterm-ls
cd <project-path>
```

**2.** Build dự án

```
make
```

**3.** Chạy dự án

```
./bin/ls
```

## 7. Minh họa các flag

| Flag | Mô tả                                            | Minh họa                                                                                              |
| ---- | ------------------------------------------------ | ----------------------------------------------------------------------------------------------------- |
| `-a` | Hiển thị tất cả file, bao gồm `.` và `..`        | ![Minh họa flag -a](https://res.cloudinary.com/dunmkypzg/image/upload/v1762114100/a_lower_gybi3p.png) |
| `-A` | Giống `-a` nhưng không hiển thị `.` và `..`      | ![Minh họa flag -A](https://res.cloudinary.com/dunmkypzg/image/upload/v1762112229/A_kn9k6q.png)       |
| `-l` | Liệt kê chi tiết                                 | ![Minh họa flag -l](https://res.cloudinary.com/dunmkypzg/image/upload/v1762112232/l_egl4cz.png)       |
| `-n` | Liệt kê chi tiết bằng UID/GID                    | ![Minh họa flag -n](https://res.cloudinary.com/dunmkypzg/image/upload/v1762112234/n_pxt1aq.png)       |
| `-i` | Hiển thị inode                                   | ![Minh họa flag -i](https://res.cloudinary.com/dunmkypzg/image/upload/v1762112230/i_vy8zbe.png)       |
| `-s` | Hiển thị block                                   | ![Minh họa flag -s](https://res.cloudinary.com/dunmkypzg/image/upload/v1762114101/s_lower_pundvb.png) |
| `-F` | Phân loại thư mục, file, link                    | ![Minh họa flag -F](https://res.cloudinary.com/dunmkypzg/image/upload/v1762112230/F_riaao7.png)       |
| `-t` | Sắp xếp theo thời gian sửa đổi                   | ![Minh họa flag -t](https://res.cloudinary.com/dunmkypzg/image/upload/v1762112234/t_kw9owb.png)       |
| `-S` | Sắp xếp theo kích thước                          | ![Minh họa flag -S](https://res.cloudinary.com/dunmkypzg/image/upload/v1762112234/S_nrpaoo.png)       |
| `-r` | Đảo ngược thứ tự sắp xếp                         | ![Minh họa flag -r](https://res.cloudinary.com/dunmkypzg/image/upload/v1762114101/r_lower_ijqaoh.png) |
| `-u` | Dựa trên access time                             | ![Minh họa flag -u](https://res.cloudinary.com/dunmkypzg/image/upload/v1762112235/u_c1vixn.png)       |
| `-c` | Dựa trên change time                             | ![Minh họa flag -c](https://res.cloudinary.com/dunmkypzg/image/upload/v1762112231/c_yhfa2v.png)       |
| `-R` | Liệt kê đệ quy                                   | ![Minh họa flag -R](https://res.cloudinary.com/dunmkypzg/image/upload/v1762112235/R_kixcfp.png)       |
| `-d` | Hiển thị chính thư mục, không liệt kê            | ![Minh họa flag -d](https://res.cloudinary.com/dunmkypzg/image/upload/v1762112229/d_btnkvc.png)       |
| `-h` | Hiển thị kích thước dễ đọc (KB, MB, GB)          | ![Minh họa flag -h](https://res.cloudinary.com/dunmkypzg/image/upload/v1762112231/h_cafee0.png)       |
| `-f` | Không sắp xếp, in toàn bộ file (bao gồm file ẩn) | ![Minh họa flag -f](https://res.cloudinary.com/dunmkypzg/image/upload/v1762114100/f_lower_mjcrpu.png) |
| `-w` | In ký tự không in được ở dạng raw                | ![Minh họa flag -w](https://res.cloudinary.com/dunmkypzg/image/upload/v1762112235/w_hh7zg1.png)       |
| `-q` | Thay ký tự không in được bằng `?`                | ![Minh họa flag -q](https://res.cloudinary.com/dunmkypzg/image/upload/v1762112234/q_qduxin.png)       |

---

## 8. Kết quả kiểm thử

### Test các cặp override

| Cặp flag    | Lệnh kiểm thử                   | Kết quả                                                                                                                                                   |
| ----------- | ------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `-w` / `-q` | `./bin/ls -qw` / `./bin/ls -wq` | Flag cuối cùng quyết định hiển thị raw hoặc `?` ![Minh họa flag -wq -qw](https://res.cloudinary.com/dunmkypzg/image/upload/v1762112236/wq_ku0iw8.png)     |
| `-l` / `-n` | `./bin/ls -ln` / `./bin/ls -nl` | Flag cuối cùng quyết định hiển thị tên hoặc UID/GID ![Minh họa flag -ln -nl](https://res.cloudinary.com/dunmkypzg/image/upload/v1762112232/ln_vvz5xf.png) |
| `-c` / `-u` | `./bin/ls -cu` / `./bin/ls -uc` | Flag cuối cùng quyết định loại thời gian dùng ![Minh họa flag -cu -uc](https://res.cloudinary.com/dunmkypzg/image/upload/v1762112229/cu_lkqcm8.png)       |
| `-R` / `-d` | `./bin/ls -Rd` / `./bin/ls -dR` | Flag cuối cùng quyết định đệ quy hoặc chỉ thư mục ![Minh họa flag -Rd -dR](https://res.cloudinary.com/dunmkypzg/image/upload/v1762112232/Rd_gtzrii.png)   |

### Test mặc định hiển thị và spacing

- Mỗi entry được in **trên một dòng riêng biệt**.
- Các thông tin từ `-i`, `-s`, `-l` được **ngăn cách bởi ít nhất một dấu cách** (`⟨blank⟩`).

![Minh họa flag -ils](https://res.cloudinary.com/dunmkypzg/image/upload/v1762112232/ils_kbtd2z.png)

---

## 8. Kết luận

Qua bài tập này, em đã:

- Hiểu sâu hơn về cách hệ điều hành Linux lưu trữ và truy cập thông tin file.
- Thành thạo việc sử dụng các API hệ thống: `opendir()`, `readdir()`, `stat()`, `lstat()`, `getpwuid()`, `getgrgid()`.
- Củng cố kỹ năng lập trình C hướng cấu trúc và tổ chức dự án với **Makefile**.
- Nắm vững cơ chế xử lý **flag override** tương tự hệ thống thực tế.

Dự án đã hoàn thiện đầy đủ các yêu cầu của đề bài, mô phỏng hành vi `ls` trong NetBSD một cách chính xác.
