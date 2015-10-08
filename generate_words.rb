
# sample size in gb
sample_size = ARGV.first.to_i * (1024 ** 3)

File.open('/usr/share/dict/words', 'r') do |f|
  lines = f.readlines
  size = 0
  while size < sample_size
    word = lines.sample
    size += word.size
    $stdout.write word
  end
end

