/*

  Generates the termdb-tables structure from an OWL ontology
  For documentation, see http://func.eva.mpg.de/doc/func.html

  Input: OWL file

  Output directory will contain three files used by FUNC
 */

				       //import org.mindswap.pellet.KnowledgeBase
				       //import org.mindswap.pellet.expressivity.*
				       //import org.mindswap.pellet.*
import java.util.logging.Logger
import org.semanticweb.owlapi.apibinding.OWLManager
import org.semanticweb.owlapi.vocab.OWLRDFVocabulary
import org.semanticweb.owlapi.model.*
import org.semanticweb.owlapi.reasoner.*
import org.semanticweb.owlapi.profiles.*
import org.semanticweb.owlapi.util.*
import org.semanticweb.owlapi.io.*
import org.semanticweb.elk.owlapi.*


String formatClassNames(String s) {
  s=s.replace("<http://purl.obolibrary.org/obo/","")
  s=s.replace(">","")
  s=s.replace("_",":")
  if (s.indexOf("/")>-1) {
    s = s.substring(s.lastIndexOf('/')+1)
  }
  if (s.indexOf('#')>-1) {
    s = s.substring(s.lastIndexOf('#')+1)
  }
  s = s.replaceAll("^a-zA-Z0-9:","")
  return s
}

def cli = new CliBuilder()
cli.with {
usage: 'Self'
  h longOpt:'help', 'this information'
  i longOpt:'input', 'OWL input file', args:1, required:true
  o longOpt:'output-directory', 'output directory',args:1, required:true
  //  t longOpt:'threads', 'number of threads', args:1
  //  k longOpt:'stepsize', 'steps before splitting jobs', arg:1
}

def opt = cli.parse(args)
if( !opt ) {
  //  cli.usage()
  return
}
if( opt.h ) {
    cli.usage()
    return
}

def owlfile = new File(opt.i)

def outdir = new File(opt.o)
def termfile = new File(outdir.getCanonicalPath()+"/term.txt")
def term2termfile = new File(outdir.getCanonicalPath()+"/term2term.txt")
def graphpathfile = new File(outdir.getCanonicalPath()+"/graph_path.txt")

OWLOntologyManager manager = OWLManager.createOWLOntologyManager()

OWLDataFactory fac = manager.getOWLDataFactory()

OWLOntology ont = manager.loadOntologyFromOntologyDocument(owlfile)

OWLReasonerFactory reasonerFactory = null

ConsoleProgressMonitor progressMonitor = new ConsoleProgressMonitor()
OWLReasonerConfiguration config = new SimpleConfiguration(progressMonitor)

OWLReasonerFactory f1 = new ElkReasonerFactory()
OWLReasoner reasoner = f1.createReasoner(ont,config)

reasoner.precomputeInferences(InferenceType.CLASS_HIERARCHY)


def counter = 0
PrintWriter term2termout = new PrintWriter(new BufferedWriter(new FileWriter(term2termfile)))
PrintWriter termout = new PrintWriter(new BufferedWriter(new FileWriter(termfile)))
PrintWriter graphpathout = new PrintWriter(new BufferedWriter(new FileWriter(graphpathfile)))

/* termout.println("\t\t\t\t\t")
term2termout.println("\t\t\t\t\t")
graphpathout.println("\t\t\t\t\t")
*/

termout.println(formatClassNames(fac.getOWLThing().toString())+"\towl:Thing\towl:Thing\t"+formatClassNames(fac.getOWLThing().toString())+"\tend")
ont.getClassesInSignature().each { cl ->
  def flag = false // found English label? otherwise take any other...
  def clname = "root"
  OWLAnnotationProperty label = fac.getOWLAnnotationProperty(OWLRDFVocabulary.RDFS_LABEL.getIRI())
  cl.getAnnotations(ont, label).each { anno ->
    if (anno.getValue() instanceof OWLLiteral) {
      OWLLiteral val = (OWLLiteral) anno.getValue();
      if (val.hasLang("en")) {
	clname = val.getLiteral()
	flag = true
      } else if (!flag) {
	clname = val.getLiteral()
      }
    }
  }
  if (clname!=null && clname.length()>0) {
    termout.println(formatClassNames(cl.toString())+"\t$clname\towl:Thing\t"+formatClassNames(cl.toString())+"\tend")
  } else {
    termout.println(formatClassNames(cl.toString())+"\t"+cl.toString()+"\towl:Thing\t"+formatClassNames(cl.toString())+"\tend")
  }
  
  //  termout.println(formatClassNames(cl.toString())+"\tb\tc\t"+formatClassNames(cl.toString())+"\tend")
  reasoner.getSuperClasses(cl, true).getFlattened().each { sup ->
    term2termout.println(counter+"\tis-a\t"+formatClassNames(sup.toString())+"\t"+formatClassNames(cl.toString()))
    counter += 1
  }

  def set1 = reasoner.getSuperClasses(cl, false).getFlattened()
  set1.addAll(reasoner.getEquivalentClasses(cl).getEntities())
  set1.each { sup ->
    graphpathout.println(counter+"\t"+formatClassNames(sup.toString())+"\t"+formatClassNames(cl.toString())+"\tis-a\t0\t0")
    counter += 1
  }
}

term2termout.flush()
termout.flush()
graphpathout.flush()

reasoner.dispose()
