
import os
import sys
import re
import codecs

# Transform list of matching strings into one, separated by a comma, string
def concatResults(list):
    return ', '.join(list)

# Fetch values from META tags
def getFromMeta(content, data):
    results = re.compile('^<META NAME="' + data + '" CONTENT="(.+)">$', re.MULTILINE | re.IGNORECASE).findall(content)
    return concatResults(results)

# Fetch values from the rest of the document
mailRegex = re.compile('\b[A-Za-z0-9](([_\.\-]?[a-zA-Z0-9]+)*)@([A-Za-z0-9]+)(([\.\-]?[a-zA-Z0-9]+)*)\.([A-Za-z]{2,})\b')
decimalsRegex = re.compile('\b[\+-]?(\d+(\.\d*)?|\.\d+)([eE][\+-]?\d+)?\b')

def countEmails(content):
    return len(set(mailRegex.findall(content)))

def countDecimals(content):
    decimalsList = decimalsRegex.findall(content)
    # return size of set containing decimal numbers constructed from parsed matching values
    return len(set(map(lambda x: float(x), decimalsList)))

def processFile(filepath):
    fp = codecs.open(filepath, 'rU', 'iso-8859-2')

    content = fp.read()
    nonMetaContent = re.search('<P>(.*?)<META', content, re.DOTALL).group(1)

    fp.close()
    print("nazwa pliku:", filepath)
    print("autor: %s" % getFromMeta(content, 'AUTOR'))
    print("dzial: %s" % getFromMeta(content, 'DZIAL'))
    print("slowa kluczowe: %s" % getFromMeta(content, 'KLUCZOWE_\d+'))
    print("liczba zdan:")
    print("liczba skrotow:")
    print("liczba liczb calkowitych z zakresu int:")
    print("liczba liczb zmiennoprzecinkowych: %d" % countDecimals(nonMetaContent))
    print("liczba dat:")
    print("liczba adresow email: %d" % countEmails(nonMetaContent))
    print("\n")



try:
    path = sys.argv[1]
except IndexError:
    print("Brak podanej nazwy katalogu")
    sys.exit(0)


tree = os.walk(path)

for root, dirs, files in tree:
    for f in files:
        if f.endswith(".html"):
            filepath = os.path.join(root, f)
            processFile(filepath)
