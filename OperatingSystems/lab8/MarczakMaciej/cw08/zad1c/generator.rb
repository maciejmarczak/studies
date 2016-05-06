#!/home/maciej/.rvm/rubies/ruby-2.3.0/bin/ruby

RECORD_LEN = 1024
RECORDS_COUNT = ARGV[1]
FILENAME = ARGV[0]
SEPARATOR = '.'

$stdout.reopen(FILENAME, "w")

(1..(RECORDS_COUNT.to_i)).each do |id|
  id_len = id.to_s.length + 1
  print id.to_s + SEPARATOR
  1.upto(RECORD_LEN - id_len) { |i| print (65 + rand(26)).chr }
end
