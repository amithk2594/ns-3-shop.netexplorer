top = '.'
out = 'build'

models = (
  'src/models/NullNetModel',
  'src/models/NamNetModel'
)

base_files = (
  'src/core/common.h',
  'src/core/common.cc',
  'src/core/algorithm.h',
  'src/core/algorithm.cc',
  'src/core/error.h',
  'src/core/error.cc',
  'src/core/gtk-util.h',
  'src/core/gtk-util.cc',
  'src/core/tree-models.h',
  'src/core/net-model.h',
  'src/core/net-model.cc',
  'src/core/scene.h',
  'src/core/scene.cc',
  'src/core/motion-manager.h',
  'src/core/motion-manager.cc',
  'src/core/motion.h',
  'src/core/motion.cc',
  'src/view/net-view.h',
  'src/view/net-view.cc',
  'src/main.cc',
)

def configure(conf):
    conf.check_tool('g++')
    conf.check_cfg(
        uselib_store  = 'GTKMM',
        package       = 'gtkmm-2.4',
        args          = '--cflags --libs',
        msg           = 'Checking for gtkmm',
        mandatory     = True
    )

def build(bld):
    sources = []

    def append_sources (files):
        for i in files:
            if i.endswith ('.cc'):
                sources.append (os.path.basename (i))

    def create_model (resources = None, files = None):
        if files:
            append_sources (files)

            bld.new_task_gen(
                'collect',
                source = files,
            )

        if resources:
            bld.new_task_gen(
                source = resources
            )

    bld.new_task_gen(
        'collect',
        source = base_files
    )

    bld.create_model = create_model;

    for i in models:
        bld.add_subdirs (i);

    append_sources (base_files)

    bld(
        uselib    = 'GTKMM',
        features  = 'cxx cprogram',
        source    =  sources,
        target    = 'netexplorer',
    )

import TaskGen
import Task
import shutil
import Utils
import os

class collect_taskgen(TaskGen.task_gen):
    def __init__(self, *args, **kwargs):
        super(collect_taskgen, self).__init__(*args, **kwargs)

    def apply(self):
        src_nodes = []
        dst_nodes = []
        for filename in set(self.to_list(self.source)):
            src_node = self.path.find_resource(filename)
            if src_node is None:
                raise Utils.WafError('source file %s not found in %s' % (filename, str(self.path)))
            dst_node = self.bld.path.find_or_declare(os.path.basename(filename))
            assert dst_node is not None
            src_nodes.append (src_node)
            dst_nodes.append (dst_node)

        task = self.create_task('collect', env = self.env)
        task.set_inputs(src_nodes)
        task.set_outputs(dst_nodes)

@TaskGen.extension('.ui')
@TaskGen.before('cxx')
def resource(self, node):
    dst = self.bld.path.find_or_declare (node.file ())
    task = self.create_task('resource', env=self.env)
    task.set_inputs([node])
    task.set_outputs([dst])

class resource_task(Task.Task):
    color = 'BLUE'
    def run(self):
        assert len(self.inputs) == len(self.outputs)
        for src, dst in zip(self.inputs, self.outputs):
            base = src.file_base().replace('-', '_').upper()
            clsname = (''.join([i.title() for i in base.split('_')]))
            src = src.srcpath(self.env)
            dst = dst.bldpath(self.env)
            outfile = file(dst, 'w')
            print >> outfile, '#ifndef UI_%s_H'%base
            print >> outfile, '#define UI_%s_H'%base
            print >> outfile, '#include <gtkmm.h>'
            print >> outfile, 'namespace ui {'
            print >> outfile, 'const Glib::ustring %s = '%clsname
            with open(src, 'rt') as infile:
                for i in infile.read().split('\n'):
                    i = i.strip ()
                    if (i):
                        print >> outfile, '"%s"' % i.replace('"', "'")
            print >> outfile, '  ;'

            print >> outfile, '}'
            print >> outfile, '#endif'
            outfile.close()
        return 0

class collect_task(Task.Task):
    color = 'BLUE'
    before = 'cxx'
    def run(self):
        assert len(self.inputs) == len(self.outputs)
        inputs = [node.srcpath(self.env) for node in self.inputs]
        outputs = [node.bldpath(self.env) for node in self.outputs]
        for src, dst in zip(inputs, outputs):
            shutil.copy2(src, dst)
        return 0
