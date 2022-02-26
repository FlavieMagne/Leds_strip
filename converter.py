cpp_lines = []

html_file = open('./index.html')
html_lines = html_file.readlines()

for line in html_lines:
    if line=="":
        pass
    else:
        line = line.replace('"', "'")
        line = line.strip()
        line='client.println("'+line
        line=line+'");'
        line=line+"\n"
        cpp_lines.append(line)

outfile = open('./buffer/script.ino', 'w')
outfile.writelines(cpp_lines)
outfile.close()