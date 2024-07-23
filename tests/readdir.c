/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readdir.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 16:18:47 by aelkheta          #+#    #+#             */
/*   Updated: 2024/07/22 16:28:15 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <dirent.h>



// int main() {
//     DIR *dir = opendir(".");
//     struct dirent *entry;
//     while ((entry = readdir(dir)) != NULL) {
//         printf("%s ", entry->d_name);
//     }
//     closedir(dir);
//     return 0;
// }



// #include <stdio.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <linux/kd.h>
// #include <sys/ioctl.h>

// int main() {
//     int fd = open("/dev/tty0", O_RDONLY);
//     int mode;
//     ioctl(fd, KDGETMODE, &mode);  // Get the keyboard mode
//     printf("Keyboard mode: %d\n", mode);
//     close(fd);
//     return 0;
// }
