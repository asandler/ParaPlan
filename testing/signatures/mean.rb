a = File.readlines(ARGV[0]).map{|line| line.to_f}
puts ARGV[0] + "\t" + (a.inject(:+) / a.size).to_s
