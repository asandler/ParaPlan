a = []
count = 0

while s = $stdin.gets
    next if s.split[0].to_f < ARGV[0].to_f

    if a.empty?
        a = s.split.map{|s| s.to_f}
    else
        s.split.map{|s| s.to_f}.each_with_index{|v, i| a[i] += v}
    end

    count += 1
end

puts a.map{|v| v /= count}.join(' ')
