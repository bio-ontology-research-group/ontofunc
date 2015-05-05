def slurper = new XmlSlurper().parseText(new URL("http://www.berkeleybop.org/ontologies/obo-all/ontology_index.xml").getText())

def downloadDir = args[0]

def map = [:]
def map2 = [:]
def prefix = "http://www.berkeleybop.org/ontologies/"
slurper.ont.each { ont ->
  String namespace = ont.namespace
  def title = ont.title
  def dirname = ont.@id.text()
  println dirname
  if (!dirname || dirname.length()==0) {
    dirname = namespace.toString()
  }
  map[dirname] = title
  map2[dirname] = namespace
  ont.export.each { exp ->
    if (exp.@format == "obo") {
      String path = exp.@path
      if (path.length()>1 && namespace?.length()>0) {
	def downloadUrl = new URL(prefix+path)
	def file = new FileOutputStream(new File(args[0]+"/$dirname"))
	def out = new BufferedOutputStream(file)
	out << downloadUrl.openStream()
	out.close()
      }
    }
  }
}

def fout = new PrintWriter(new FileWriter(args[0]+"/obo-index.txt"))
map.each { ns, ti ->
  
  fout.println("$ns\t$ti\t"+map2[ns])
}
fout.flush()
fout.close()
